#include"appl_main.h"
#include"nvm_hwab.h"
uint32 sampleData_prev = 0;
uint32 sampleData = 0;
sample_block_t Read_Buffer_Sample_Data_2 = {0,0};
uint32 Read_Buffer_Sample_Data = 0;

blockAddress_t blockconfig[NUM_BLOCKS] = 
{
    {BLOCK_BASE,BLOCK_BASE,BLOCK_BASE,BLOCK_BASE,sizeof(uint32),NVM_BLOCK_1,&Read_Buffer_Sample_Data},
    {BLOCK_BASE,BLOCK_BASE,BLOCK_BASE,BLOCK_BASE,sizeof(sample_block_t),NVM_BLOCK_2,(uint32*)&Read_Buffer_Sample_Data_2}
};

void application_main(void)
{
    static uint8 ret = 0;
    static uint8 runstate = 0;

    if(sampleData != sampleData_prev)
    {
        ret = write_block(0,&sampleData);
        if(ret == E_OK)
        {
            sampleData_prev = sampleData;
        }
        else if(ret == NVM_PENDING)
        {
            nvm_write_status();
        }  
    }

    ret = nvm_read(0,&Read_Buffer_Sample_Data, blockconfig[0].Read_Address);
}


void nvm_init(void)
{
    drv_init();
    
    nvm_createBlockRegions();

    search_blockAddresses();
}



