#ifndef NVM_HWAB_H_
#define NVM_HWAB_H_

    #include"GlobalTypes.h"

    #define META_HANDLING       0u
    #define WRITING             1u
    #define NVM_PENDING         2u

    typedef struct {
        uint32 BlockRegionStart;
        uint32 BlockRegionEnd;
        uint32 Read_Address;
        uint32 write_Address;
        uint8 datasize;
        uint8 id;
        uint32* readbuffer;
    } blockAddress_t;

    typedef struct block_nvm{
        uint32 Read_Address;
        uint32 write_Address;
        uint8 blockid;
        uint8* data;
    } nvm_block_t;

    uint8 write_block(uint8 id, uint8* buffer);



#endif