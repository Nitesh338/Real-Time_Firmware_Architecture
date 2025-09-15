#include "FreeRTOS.h"   /*for the definition of uint32_t*/
#include "OS_Init.h"

extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;

extern void reset_handler(void);
extern void HardFault_handler(void);

void reset_handler(void)
{
    uint32_t *bss = &_sbss;
    while(bss < &_ebss){ *bss++ = 0;}

    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata){*dst++ = *src++;}

    Init();
}

void HardFault_handler()
{
    volatile uint32_t hfsr = SCB->HFSR;
    volatile uint32_t cfsr = SCB->CFSR;
    volatile uint32_t bfar = SCB->BFAR;
    volatile uint32_t mmfar = SCB->MMFAR;
    __asm volatile
    (
        "TST lr, #4              \n"       // Test bit 2 of LR (EXC_RETURN)
        "ITE EQ                  \n"
        "MRSEQ r0, MSP           \n"       // If bit 2 = 0 → use MSP (Handler mode)
        "MRSNE r0, PSP           \n"       // If bit 2 = 1 → use PSP (Thread mode, task)
        "B hard_fault_handler_c  \n"
    );
}

void hard_fault_handler_c(uint32_t *stack_address)
{
    uint32_t r0  = stack_address[0];
    uint32_t r1  = stack_address[1];
    uint32_t r2  = stack_address[2];
    uint32_t r3  = stack_address[3];
    uint32_t r12 = stack_address[4];
    uint32_t lr  = stack_address[5];
    uint32_t pc  = stack_address[6];  // <=== PC at time of crash
    uint32_t psr = stack_address[7];

}

void* memset(void* stack, int BYTE, unsigned int stackdepth)
{
    unsigned char *ptr = (unsigned char *)stack;
    unsigned char data = (unsigned char)BYTE;

    while(stackdepth>0)
    {
        *ptr = data;
        stackdepth--;
        ptr++;
    }
}

void *memcpy (void *__restrict __dest, const void *__restrict __src, size_t __n)
{
    unsigned char *d = (unsigned char *)__dest;
    const unsigned char *s = (const unsigned char *)__src;

    while (__n--) {
        *d++ = *s++;
    }

    return __dest;
}