#ifndef APPL_MAIN_H_
#define APPL_MAIN_H_

    #include"GlobalTypes.h"
    #include"nvm_drv.h"
    #include"nvm_hwab.h"

    typedef struct {
        uint32 sample1;
        uint64 sample2;
    } sample_block_t;
    
    extern sample_block_t Read_Buffer_Sample_Data_2;

    extern uint32 sampleData_prev;
    extern uint32 sampleData;
    extern uint32 Read_Buffer_Sample_Data;

    void application_main(void);
    void nvm_init(void);
#endif