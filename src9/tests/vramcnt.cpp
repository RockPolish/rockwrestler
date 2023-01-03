// tests VRAMCNT
#include "../inc.h"

void run_tests_vramcnt()
{
    // tell ARM7 to await read/write commands
    ARM7_start_rw();

    // first, set all 9 VRAM banks to LCDC mode and write a magic value to start/end
    REG_VRAMCNT_A = REG_VRAMCNT_B = REG_VRAMCNT_C = REG_VRAMCNT_D = REG_VRAMCNT_E = 0x80;
    REG_VRAMCNT_F = REG_VRAMCNT_G = REG_VRAMCNT_H = REG_VRAMCNT_I = 0x80;

    // bank    size             LCDC address range
    // A       128K  0    -     6800000h-681FFFFh
    // B       128K  0    -     6820000h-683FFFFh
    // C       128K  0    -     6840000h-685FFFFh
    // D       128K  0    -     6860000h-687FFFFh
    // E       64K   0    -     6880000h-688FFFFh
    // F       16K   0    -     6890000h-6893FFFh
    // G       16K   0    -     6894000h-6897FFFh
    // H       32K   0    -     6898000h-689FFFFh
    // I       16K   0    -     68A0000h-68A3FFFh
    *((vu32*)0x6800000) = VRAM_A_START;
    *((vu32*)0x681FFFC) = VRAM_A_END;
    *((vu32*)0x6820000) = VRAM_B_START;
    *((vu32*)0x683FFFC) = VRAM_B_END;
    *((vu32*)0x6840000) = VRAM_C_START;
    *((vu32*)0x685FFFC) = VRAM_C_END;
    *((vu32*)0x6860000) = VRAM_D_START;
    *((vu32*)0x687FFFC) = VRAM_D_END;
    // TODO E, F, G, H, I


    // try to read back those values
    if (*((vu32*)0x6800000) != VRAM_A_START) fail_test(0x000);
    if (*((vu32*)0x681FFFC) != VRAM_A_END) fail_test(0x001);
    if (*((vu32*)0x6820000) != VRAM_B_START) fail_test(0x002);
    if (*((vu32*)0x683FFFC) != VRAM_B_END) fail_test(0x003);
    if (*((vu32*)0x6840000) != VRAM_C_START) fail_test(0x004);
    if (*((vu32*)0x685FFFC) != VRAM_C_END) fail_test(0x005);
    if (*((vu32*)0x6860000) != VRAM_D_START) fail_test(0x006);
    if (*((vu32*)0x687FFFC) != VRAM_D_END) fail_test(0x007);

    // ARM7 does not have any VRAM at the start: make it read from its VRAMSTAT, should be 0
    if (ARM7_read_byte(0x4000240) != 0) fail_test(0x008);

    // give VRAM C to ARM7 with offset 0 as WRAM
    REG_VRAMCNT_C = 0x82;

    // ARM7 can now access it at 6000000h+(20000h*OFS.0), OFS = 0
    //  + ARM7 VRAMSTAT (240h) should be 0x01
    if (ARM7_read_byte(0x4000240) != 1) fail_test(0x009);

    // read start/end
    if (ARM7_read_word(0x6000000) != VRAM_C_START) fail_test(0x00A);
    if (ARM7_read_word(0x601FFFC) != VRAM_C_END) fail_test(0x00B);

    // change offset to 1
    REG_VRAMCNT_C = 0x8A;

    // check VRAMSTAT, read start/end with 128 KB offset
    if (ARM7_read_byte(0x4000240) != 1) fail_test(0x00C);
    if (ARM7_read_word(0x6020000) != VRAM_C_START) fail_test(0x00D);
    if (ARM7_read_word(0x603FFFC) != VRAM_C_END) fail_test(0x00E);

    // take away bank C, give bank D
    REG_VRAMCNT_C = 0; // just disable it
    REG_VRAMCNT_D = 0x82;

    // check VRAMSTAT, read start/end
    if (ARM7_read_byte(0x4000240) != 2) fail_test(0x00F);
    if (ARM7_read_word(0x6000000) != VRAM_D_START) fail_test(0x010);
    if (ARM7_read_word(0x601FFFC) != VRAM_D_END) fail_test(0x011);

    // enable bank C with offset 1
    REG_VRAMCNT_C = 0x8A;

    // check VRAMSTAT, read start/end of both banks
    if (ARM7_read_byte(0x4000240) != 3) fail_test(0x012);
    if (ARM7_read_word(0x6000000) != VRAM_D_START) fail_test(0x013);
    if (ARM7_read_word(0x601FFFC) != VRAM_D_END) fail_test(0x014);
    if (ARM7_read_word(0x6020000) != VRAM_C_START) fail_test(0x015);
    if (ARM7_read_word(0x603FFFC) != VRAM_C_END) fail_test(0x016);

    // set bank A to MST = 1 (engine A BG), offset 0
    REG_VRAMCNT_A = 0x81;

    // 6000000h+(20000h*OFS)
    if (*((vu32*)0x6000000) != VRAM_A_START) fail_test(0x017);
    if (*((vu32*)0x601FFFC) != VRAM_A_END) fail_test(0x018);

    // bank D: MST = 1 (engine A BG), offset 1
    REG_VRAMCNT_D = 0x89;

    // 6000000h+(20000h*OFS)
    if (*((vu32*)0x6020000) != VRAM_D_START) fail_test(0x019);
    if (*((vu32*)0x603FFFC) != VRAM_D_END) fail_test(0x01A);


    // reset bank A to LCDC as we use it to draw the screen
    REG_VRAMCNT_A = 0x80;

    // tell ARM7 we're done
    ARM7_end_rw();

    // TODO. exhaustively check all MST/OFS combinations per bank, check VRAM bank overlapping
}
