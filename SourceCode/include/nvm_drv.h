#include"GlobalTypes.h"
#include"SW_Config.h"

#define FLASH_ACR       (*(volatile uint32*)0x40023C00)
#define LATENCY_MASK    (uint32)0x0F
#define LATENCY         (uint32)WAIT_STATE_VALUE
#define PRFTEN          8
#define PRFTEN_MASK     (uint32)(PREFETCH_STATUS << PRFTEN)

#define FLASH_KEYR      (*(volatile uint32*)0x40023C04)
#define KEY1            0x45670123
#define KEY2            0xCDEF89AB

#define FLASH_SR        (*(volatile uint32*)0x40023C0C)
#define BUSY            16
#define EOP             0

#define FLASH_CR        (*(volatile uint32*)0x40023C10)
#define STRT            16
#define LOCK            31
#define SNB             Sector_num
#define PSIZE           8
#define MASS_ERASE      2
#define SECTOR_ERASE    1
#define PROG            0

#define b_FREE          (boolean)1
#define b_BUSY          (boolean)0

extern boolean nvm_driverwriteStatus;
boolean nvm_write(void);
boolean nvm_read(uint8 id, uint8* bufferptr, uint8* flashaddrptr);

void search_blockAddresses(void);

void nvm_createBlockRegions(void);