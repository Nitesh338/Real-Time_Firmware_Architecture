#include"nvm_drv.h"
#include"GlobalTypes.h"
#include"nvm_hwab.h"

boolean Free_sectors[8] = {0};
uint8 sector_num = 0;
boolean nvm_driverwriteStatus = b_FREE;


void nvm_createBlockRegions(void)
{
    uint32 size_regions = (128*1024)/ NUM_BLOCKS;
    uint8 index = 0;
    uint32 blockStartAddress = BLOCK_BASE;
    for(index = 0;index<NUM_BLOCKS;index++)
    {
        blockconfig[index].BlockRegionStart = blockStartAddress;
        blockconfig[index].BlockRegionEnd   = blockStartAddress + size_regions-1;

        blockStartAddress+= (size_regions);
    }
}

uint32* search(uint32 Address1, uint32 Address2)
{
    uint32* MiddleAddress = NULL;
    uint32* ret = NULL;

    MiddleAddress = Address1+((Address2 - Address1)/2);
    if((MiddleAddress == Address1)||(MiddleAddress == Address2))
    {
        ret = MiddleAddress;
    }
    else
    {
        if(*MiddleAddress == 0xFFFFFFFF)
        {
            ret = search(Address1,MiddleAddress);
        }
        else
        {
            ret = search(MiddleAddress,Address2);        
        }
    }
    return ret;
}

void search_blockAddresses(void)
{
    uint8 index;
    uint32* LastUpdatedAddress = NULL;
    uint8 blocksize;
    uint8 padding;
    uint8 blockwords;
    uint32 Tempaddr = 0;
    for(index = 0;index<NUM_BLOCKS;index++)
    {
        LastUpdatedAddress = search(blockconfig[index].BlockRegionStart, blockconfig[index].BlockRegionEnd);
        /*lets make it align to 4*/
        Tempaddr = LastUpdatedAddress;
        Tempaddr&=(~0x03);
        LastUpdatedAddress = Tempaddr;
        blockconfig[index].write_Address = LastUpdatedAddress+1;

        blocksize = blockconfig[index].datasize + 9;
        if(blocksize%4 !=0)
        {
            padding = 4- (blocksize%4);
            blocksize+=padding;
        }
        blockwords = blocksize/4;
        blockconfig[index].Read_Address  = LastUpdatedAddress - (blockwords - 1);

    }
}



void unlock_flash(void)
{
    FLASH_KEYR|=KEY1;
    FLASH_KEYR|=KEY2;
}

boolean Is_drv_busy(void)
{
    boolean status;
    status = (boolean)((FLASH_SR>>16)&0x01);
    return status;
}

void lock_controlregister(void)
{
    FLASH_CR|=(uint32)(1<<LOCK);
}

boolean start_erase(void)
{
    FLASH_CR|=(uint32)((1<<SECTOR_ERASE)|(1<<STRT));

    while(b_TRUE == Is_drv_busy());

    lock_controlregister();
    return b_TRUE;
}

boolean erase_sector(uint8 sector)
{
    boolean drv_status = b_FALSE;
    /*check whether driver is not in busy state*/
    drv_status = Is_drv_busy();
    if(drv_status == b_FALSE)
    {
        sector_num = 0;
        FLASH_CR|=(uint32)(sector<<3);
        start_erase();
    }
    else
    {
        return b_FALSE;
    }
    Free_sectors[sector] = b_TRUE;
    return b_TRUE;
}

void drv_init(void)
{
    FLASH_ACR|=(LATENCY | PRFTEN_MASK);
}

void nvm_write_status(void)
{
    if(b_FALSE == Is_drv_busy())
    {
        nvm_driverwriteStatus = b_FREE;
        FLASH_CR &= ~(1 << PROG);
        lock_controlregister();
    }  
}

boolean nvm_write(void)
{
    boolean drv_status = b_FALSE;
    uint8 progsize = 0;

    if(nvm_driverwriteStatus == b_FREE)
    {
        unlock_flash();
        FLASH_CR|=(1<<PROG);
        progsize = 0u;
        FLASH_CR|=(uint32)(progsize << PSIZE);
        return b_TRUE;
     }
    return b_FALSE;
}

boolean nvm_erase(uint8 sector)
{
    boolean ret;
    unlock_flash();
    ret = erase_sector(sector);
    return ret;
}

boolean nvm_read(uint8 id,uint8* bufferptr, uint8* flashaddrptr)
{
    uint8* datafield = flashaddrptr + 9;
    uint8 size = blockconfig[id].datasize;
    uint8 FetchedID = 0;

    if(b_FALSE == Is_drv_busy())
    {
        memcpy(FetchedID, flashaddrptr+8, sizeof(uint8));
        if(FetchedID == id)
        {
            while(size--)
            {
                memcpy(bufferptr++, datafield++, sizeof(*bufferptr));
            }   
            return b_TRUE;         
        }
        else
        {
            return b_FALSE;
        }

    }
}














