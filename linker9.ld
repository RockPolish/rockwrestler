OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(main)

MEMORY
{
    rom : ORIGIN = 0x02000100, LENGTH = 0x300000  /* 4 MB - 256 bytes - some stuff at the end.. this probably does not matter much? */
}

SECTIONS
{
    .text : { *(.text*) *(.bss*) } > rom   /* honestly not sure how this file works, but it works */
}
