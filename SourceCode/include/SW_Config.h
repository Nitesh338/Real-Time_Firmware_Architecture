#ifndef SW_CONFIG_H_
#define SW_CONFIG_H_

    #include"nvm_hwab.h"
    #include"appl_main.h"

    #define WAIT_STATE_VALUE    0
    #define PREFETCH_STATUS     0

    /*===================================
            NvM configuration
    ===================================*/
    #define BLOCK_BASE   0x08060000UL
    #define NUM_BLOCKS  2u

    #define NVM_BLOCK_1 0
    #define NVM_BLOCK_2 1l

   extern blockAddress_t blockconfig[NUM_BLOCKS];

#endif