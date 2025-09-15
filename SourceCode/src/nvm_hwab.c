
#include"nvm_drv.h"
#include"appl_main.h"

static uint8* BLOCK_HEAD;

uint8 write_block(uint8 id, uint8* buffer)
{
    uint8 ret;
    uint8 padding = 0;
    boolean write_status;
    static nvm_block_t Temp_Block = {0};
    static uint8* Temp_Block_ptr = NULL;
    uint8 BlockSize = 0;
    static uint8 counter;
    static boolean state;
    uint8 datasize = blockconfig[id].datasize;

    if((datasize<= 0u)||(buffer == NULL)||(id>NUM_BLOCKS))
    {
        ret = E_NOT_OK;
    }

    switch(state)
    {
        case META_HANDLING:
            //datasize+=sizeof(nvm_block_t);
            BlockSize = (datasize + 9u)*8;
            if(BlockSize % 32u != 0)
            {
                padding  = 32u - (BlockSize % 32u);
                BlockSize+=padding;
            }
           // Temp_Block = NULL;
            BLOCK_HEAD = (volatile uint8*)(blockconfig[id].write_Address);
            Temp_Block.blockid = id;
            Temp_Block.Read_Address = BLOCK_HEAD;
            Temp_Block.write_Address = BLOCK_HEAD + BlockSize/8;
            blockconfig[id].write_Address = Temp_Block.write_Address;
            blockconfig[id].Read_Address = BLOCK_HEAD;
            Temp_Block.data = buffer;
            Temp_Block_ptr = &Temp_Block;
            state = 1;
            counter = 0;
        case WRITING:
            if(counter < (datasize + 9u))
            {
                write_status = nvm_write();
                if(write_status == b_TRUE)
                {
                    counter++;
                    if(counter < 10)
                    {
                        *BLOCK_HEAD++ = *(Temp_Block_ptr++);
                    }
                    else
                    {
                        *BLOCK_HEAD++ = *(buffer++);
                    }   
                    nvm_driverwriteStatus = b_BUSY; 
                }
                ret = NVM_PENDING;
            }
            else
            {
                state = 0;
                ret = E_OK;
            }
            break;
        default:
            break;
    }
    return ret;
}
