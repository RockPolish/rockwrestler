OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(main)

MEMORY
{
    rom : ORIGIN = 0x03800100, LENGTH = 0x10000 - 0x200
}

SECTIONS
{
    .text : { *(.text*) *(.bss*) } > rom   /* honestly not sure how this file works, but it works */
}
