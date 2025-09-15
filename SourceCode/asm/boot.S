.section .vectors
vector_table:
    .word __STACK
    .word reset_handler
    .word Default
    .word HardFault_handler
    .word Default
    .word Default
    .word Default
    .word Default
    .word Default
    .word Default
    .word Default
    .word vPortSVCHandler
    .word Default
    .word Default
    .word xPortPendSVHandler
    .word xPortSysTickHandler
    .word Default

.section .text
    .align 4
    .type Default, %function
Default:
    b .
