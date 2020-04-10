MEMORY
{
    ram : ORIGIN = 0x20000000, LENGTH = 0x10000
}

SECTIONS
{
    .text : { *(.text*) } > ram
    .bss : { *(.bss*) } > ram
    .data : { *(.data*) } > ram
}
