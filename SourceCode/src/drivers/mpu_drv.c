#include"GlobalTypes.h"
#include "FreeRTOS.h"
#include "task.h"

#define MPU_TYPE *(volatile uint32*)0xE00ED90

#define MPU_CTRL *(volatile uint32*)0xE00ED94
#define MPU_ENABLE  (1<<0)
#define HFNMIENA    (1<<1)
#define PRIVDEFENA  (1<<2)

#define MPU_RNR  *(volatile uint32*)0xE00ED98
#define REGION   

#define MPU_RBAR *(volatile uint32*)0xE00ED9C
#define REGION_RBAR 
#define VALID


#define MPU_RASR *(volatile uint32*)0xE00EDA0
#define ENABLE_RSAR 0
#define SIZE        1
#define SRD         8
#define B           16
#define C           17  
#define S           18
#define TEX         19
#define AP          24
#define XN          28

#define NO_ACCESS       0
#define PRV_RW          1
#define PRV_RW_U_RO     2
#define FUL_ACCESS      3
#define PRV_RO          4
#define RO              5

#define FLASH_BASE      0x08000000
#define FLASH_SIZE      (512*1024)
#define SRAM_BASE       0x20000000
#define SRAM_SIZE       (128*1024)

/* CMSIS barriers (if not using CMSIS include these prototypes) */
extern void __DSB(void);
extern void __ISB(void);


static int size_to_sizefield(uint32 size_bytes){
    if (size_bytes < 32) return -1;
    if ((size_bytes & (size_bytes - 1)) != 0) return -1;
    int log2 = 31 - __builtin_clz(size_bytes);
    return log2 - 1;
}

static boolean mpu_program_region(uint32 region_index,
                               uint32 base_addr,
                               uint32 size_bytes,
                               uint8 srd_mask,
                               uint8 ap,
                               uint8 tex, uint8 s, uint8 c, uint8 b,
                               uint8 xn)
{
    int sz = size_to_sizefield(size_bytes);
    if (sz < 0 || region_index > 7) return FALSE;
    if ((base_addr & (size_bytes - 1)) != 0) return FALSE; /* alignment */

    MPU_RNR = region_index;
    MPU_RBAR = (base_addr & 0xFFFFFFE0UL);  /* RBAR lower bits ignored by hardware */

    uint32 rasr = 0;
    rasr |= (1U << ENABLE_RSAR);
    rasr |= ((uint32)sz << SIZE);
    rasr |= ((uint32)(srd_mask & 0xFF) << SRD);
    rasr |= ((uint32)(b & 1U) << B);
    rasr |= ((uint32)(c & 1U) << C);
    rasr |= ((uint32)(s & 1U) << S);
    rasr |= ((uint32)(tex & 7U) << TEX);
    rasr |= ((uint32)(ap & 7U) << AP);
    rasr |= ((uint32)(xn & 1U) << XN);

    MPU_RASR = rasr;
    return b_TRUE;
}

void mpu_init_system(void)
{
    /* Enable MemManage faults so violations generate MemManage handler */
   // SCB_SHCSR |= (1U << 16); /* MEMFAULTENA */

    /* Disable MPU while programming */
    MPU_CTRL = 0;
    __DSB(); __ISB();

    /* Region 0: NULL pointer guard at 0x00000000, size 32B, no access */
    mpu_program_region(0, 0x00000000UL, 32, 0x00, NO_ACCESS, 0, 0, 0, 0, 0);

    /* Region 1: Flash region - make RO for unprivileged, executable (XN=0)
       Use normal cached (TEX=0, C=1, B=0) */
    /* choose a pow2 covering FLASH_SIZE (e.g., 256KB -> 262144 -> next pow2 262144) */
    /* for simplicity, use the exact FLASH_SIZE if it is pow2; otherwise pick the next pow2 in your edit. */
    mpu_program_region(1, FLASH_BASE, FLASH_SIZE, 0x00, PRV_RW_U_RO, 0, 0, 1, 0, 0);

    /* Region 2: SRAM region - mark privileged-only and XN (no execute for data) */
    mpu_program_region(2, SRAM_BASE, SRAM_SIZE, 0x00, PRV_RW, 0, 0, 1, 0, 1);

    /* Region 3: Peripherals (device memory) at 0x40000000 - mark device (TEX/C/B = 0, S=1) and XN */
    mpu_program_region(3, 0x40000000UL, 0x20000000UL, 0x00, FUL_ACCESS, 0, 1, 0, 0, 1);

    /* Enable MPU, keep privileged default map so kernel can run */
    __DSB(); __ISB();
    MPU_CTRL = MPU_ENABLE | PRIVDEFENA;
    __DSB(); __ISB();
}

/* Trusted task - runs privileged */
static void TrustedTask(void *pvParameters)
{
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


static void UntrustedTask(void *pvParameters)
{
    volatile uint32 *allowed = (volatile uint32*)(SRAM_BASE + 0x1000); /* this address should be in allowed region */
    volatile uint32 *forbidden = (volatile uint32*)(SRAM_BASE + SRAM_SIZE - 4); /* intentionally outside allowed region */

    /* write allowed location - should work */
    *allowed = 0x11111111;

    /* small delay then attempt forbidden write to provoke MemManage */
    vTaskDelay(pdMS_TO_TICKS(500));

    /* Illegal access: should cause MemManage fault */
    *forbidden = 0xDEADBEEF;

    /* If we get here, MPU didn't fault — spin so you can see unexpected behavior */
    // for (;;)
    // {
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }
}

/* Create the tasks using FreeRTOS MPU APIs */
void create_tasks_and_start(void)
{
    /* Initialize system MPU first */
    mpu_init_system();

    /* Create privileged task normally */
    xTaskCreate(TrustedTask, "Trusted", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);

    /* Create unprivileged (MPU) task using static API */
    static StackType_t untrustedStack[256];
    static StaticTask_t untrustedTCB;

    /* Fill MemoryRegion_t entries (FreeRTOS MPU port type) */
    MemoryRegion_t regions[3];

    /* Region 0: code region in Flash (RO, executable) - give whole FLASH for simplicity */
    regions[0].pvBaseAddress = (void*)FLASH_BASE;
    regions[0].ulLengthInBytes = FLASH_SIZE;
    regions[0].ulParameters = portMPU_REGION_READ_ONLY; /* macro from FreeRTOS MPU port */

    /* Region 1: stack/data region: pick a 4KB chunk inside SRAM_BASE + offset (must match your mpu settings alignments)
       NOTE: FreeRTOS port will convert ulLengthInBytes to MPU region(s) appropriately */
    regions[1].pvBaseAddress = (void*)(SRAM_BASE + 0x1000); /* start of allowed stack/data */
    regions[1].ulLengthInBytes = 4 * 1024; /* 4 KB */
    regions[1].ulParameters = portMPU_REGION_READ_WRITE;

    /* Region 2: not used */
    regions[2].pvBaseAddress = NULL;
    regions[2].ulLengthInBytes = 0;
    regions[2].ulParameters = 0;

    TaskParameters_t params =
    {
        .pvTaskCode = UntrustedTask,
        .pcName = "Untrusted",
        .usStackDepth = sizeof(untrustedStack) / sizeof(StackType_t),
        .pvParameters = NULL,
        .uxPriority = tskIDLE_PRIORITY + 1,
        .puxStackBuffer = untrustedStack,
        .xRegions = { regions[0], regions[1], regions[2] }
    };

    /* Create the MPU-restricted task */
    xTaskCreateRestrictedStatic(&params, &untrustedTCB);

    /* Start scheduler */
    vTaskStartScheduler();
}

