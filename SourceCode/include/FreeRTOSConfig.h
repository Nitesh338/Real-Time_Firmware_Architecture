#ifndef _CONFIG_H_
#define _CONFIG_H_

    #define INCLUDE_xTaskDelayUntil     1
    #define configUSE_TIMERS            1
    #define configTIMER_TASK_PRIORITY   1
    #define configTIMER_QUEUE_LENGTH    10
    #define configTIMER_TASK_STACK_DEPTH 128
    #define configTICK_RATE_HZ          1000
    #define configCPU_CLOCK_HZ          8000000
    #define configTOTAL_HEAP_SIZE       0x1A00
    #define configMAX_PRIORITIES        5
    #define configUSE_PREEMPTION        1
    #define configUSE_IDLE_HOOK         0
    #define configUSE_TICK_HOOK         0
    #define configMINIMAL_STACK_SIZE        0x0100
    #define configMAX_SYSCALL_INTERRUPT_PRIORITY    5
    #define configTICK_TYPE_WIDTH_IN_BITS   TICK_TYPE_WIDTH_32_BITS
    #define configENABLE_MPU            1

#endif