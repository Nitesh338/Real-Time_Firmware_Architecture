#ifndef __APPL_INIT_H
#define __APPL_INIT_H

    #define SCB_BASE        (0xE000ED00UL)
    #define SCB             ((SCB_Type *)SCB_BASE)

    typedef struct{
        volatile uint32_t CPUID;   // 0x00: CPUID Base Register
        volatile uint32_t ICSR;    // 0x04: Interrupt Control and State Register
        volatile uint32_t VTOR;    // 0x08: Vector Table Offset Register
        volatile uint32_t AIRCR;   // 0x0C: Application Interrupt and Reset Control
        volatile uint32_t SCR;     // 0x10: System Control Register
        volatile uint32_t CCR;     // 0x14: Configuration and Control Register
        volatile uint8_t  SHP[12]; // 0x18: System Handlers Priority Registers
        volatile uint32_t SHCSR;   // 0x24: System Handler Control and State Register
        volatile uint32_t CFSR;    // 0x28: Configurable Fault Status Register
        volatile uint32_t HFSR;    // 0x2C: HardFault Status Register
        volatile uint32_t DFSR;    // 0x30: Debug Fault Status Register
        volatile uint32_t MMFAR;   // 0x34: MemManage Fault Address Register
        volatile uint32_t BFAR;    // 0x38: BusFault Address Register
        volatile uint32_t AFSR;    // 0x3C: Auxiliary Fault Status Register
        volatile uint32_t PFR[2];  // 0x40: Processor Feature Register
        volatile uint32_t DFR;     // 0x48: Debug Feature Register
        volatile uint32_t ADR;     // 0x4C: Auxiliary Feature Register
        volatile uint32_t MMFR[4]; // 0x50: Memory Model Feature Register
        volatile uint32_t ISAR[5]; // 0x60: Instruction Set Attributes Register
        uint32_t RESERVED0[5];
        volatile uint32_t CPACR;   // 0x88: Coprocessor Access Control Register
    } SCB_Type;

    
    void Init(void);

#endif