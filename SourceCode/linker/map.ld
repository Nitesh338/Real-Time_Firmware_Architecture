MEMORY
{
    FLASH : ORIGIN = 0x08000000, LENGTH = 128K
    RAM : ORIGIN = 0x20000000, LENGTH = 8K
}

SECTIONS
{
    .text : {
        *(.vectors)
        *(.text*)
        *(.rodata)
    } > FLASH

    .data : {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > RAM AT > FLASH

    .bss : {
        _sbss = .;
        *(.bss*)
        _ebss = .;
    } >RAM

    _sidata = LOADADDR(.data);

    MEMTOP = ORIGIN(RAM) + LENGTH(RAM);
    PROVIDE( __STACK = MEMTOP - 4);
}