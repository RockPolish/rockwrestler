// tests WRAMCNT/allocating the 32 KB memory block (partially) to ARM7
#include "../inc.h"

void run_tests_wramcnt()
{
    // get ARM7 ready for read/write commands
    ARM7_start_rw();

    // NDS9 - WRAMCNT - 0: 32 KB (ARM9)
    //                  1: second half (ARM9),  first half (ARM7)   
    //                  2:  first half (ARM9), second half (ARM7)
    //                  3: 32 KB (ARM7)
    // deallocation: ARM9: undefined (3******)
    //               ARM7: area is 03000000-037FFFFF, not allocated -> mirror of ARM7-WRAM (03800000-03FFFFFF)

    // allocate all to ARM9
    REG_WRAMCNT = 0;

    // write magic values at start/end of each 16 KB section
    *((vu32*)0x3000000) = SHWRAM_START1;
    *((vu32*)0x3003FFC) = SHWRAM_END1;
    *((vu32*)0x3004000) = SHWRAM_START2;
    *((vu32*)0x3007FFC) = SHWRAM_END2;

    // try to read it back
    if (*((vu32*)0x3000000) != SHWRAM_START1) fail_test(0x000);
    if (*((vu32*)0x3003FFC) != SHWRAM_END1) fail_test(0x001);
    if (*((vu32*)0x3004000) != SHWRAM_START2) fail_test(0x002);
    if (*((vu32*)0x3007FFC) != SHWRAM_END2) fail_test(0x003);
    // + from a mirror
    if (*((vu32*)0x3020000) != SHWRAM_START1) fail_test(0x004);
    if (*((vu32*)0x3023FFC) != SHWRAM_END1) fail_test(0x005);
    if (*((vu32*)0x3024000) != SHWRAM_START2) fail_test(0x006);
    if (*((vu32*)0x3027FFC) != SHWRAM_END2) fail_test(0x007);

    // check ARM7 WRAMSTAT: should be 0
    if ((ARM7_read_byte(0x4000241) & 3) != 0) fail_test(0x008);

    // ARM7 should read mirrors of its own WRAM in this area
    // first, make it write start/end value to its own 64 KB WRAM
    ARM7_write_word(0x3800000, ARM7RAM_START);
    ARM7_write_word(0x380FFFC, ARM7RAM_END);

    // quickly check to make sure it was written to ARM7RAM correctly
    // should not be necessary as the ARM7 is running code from there
    // if the memory region isn't working, we would never have gotten this far
    if (ARM7_read_word(0x3800000) != ARM7RAM_START) fail_test(0x009);
    if (ARM7_read_word(0x380FFFC) != ARM7RAM_END) fail_test(0x00A);

    // now make ARM7 read from the shared WRAM region (03000000-037FFFFF)
    // should contain mirror of its own WRAM -> the magic values should be there
    if (ARM7_read_word(0x3000000) != ARM7RAM_START) fail_test(0x00B);
    if (ARM7_read_word(0x300FFFC) != ARM7RAM_END) fail_test(0x00C);
    // + read from mirror
    if (ARM7_read_word(0x3400000) != ARM7RAM_START) fail_test(0x00D);
    if (ARM7_read_word(0x340FFFC) != ARM7RAM_END) fail_test(0x00E);


    // switch to mode 1: we get second half, ARM7 gets first half
    REG_WRAMCNT = 1;

    // check WRAMSTAT
    if ((ARM7_read_byte(0x4000241) & 3) != 1) fail_test(0x00F);

    // make sure we can read the second half anywhere
    if (*((vu32*)0x3000000) != SHWRAM_START2) fail_test(0x010);
    if (*((vu32*)0x3003FFC) != SHWRAM_END2) fail_test(0x011);
    if (*((vu32*)0x304C000) != SHWRAM_START2) fail_test(0x012);
    if (*((vu32*)0x304FFFC) != SHWRAM_END2) fail_test(0x013);

    // and ARM7 should be able to read the first half
    if (ARM7_read_word(0x3000000) != SHWRAM_START1) fail_test(0x014);
    if (ARM7_read_word(0x3003FFC) != SHWRAM_END1) fail_test(0x015);
    if (ARM7_read_word(0x300C000) != SHWRAM_START1) fail_test(0x016);
    if (ARM7_read_word(0x300FFFC) != SHWRAM_END1) fail_test(0x017);


    // switch to mode 2: we get first half, ARM7 gets second half
    REG_WRAMCNT = 2;

    // check WRAMSTAT
    if ((ARM7_read_byte(0x4000241) & 3) != 2) fail_test(0x018);

    // make sure we can read the first half anywhere
    if (*((vu32*)0x3000000) != SHWRAM_START1) fail_test(0x019);
    if (*((vu32*)0x3003FFC) != SHWRAM_END1) fail_test(0x01A);
    if (*((vu32*)0x304C000) != SHWRAM_START1) fail_test(0x01B);
    if (*((vu32*)0x304FFFC) != SHWRAM_END1) fail_test(0x01C);

    // and ARM7 should be able to read the second half
    if (ARM7_read_word(0x3000000) != SHWRAM_START2) fail_test(0x01D);
    if (ARM7_read_word(0x3003FFC) != SHWRAM_END2) fail_test(0x01E);
    if (ARM7_read_word(0x300C000) != SHWRAM_START2) fail_test(0x01F);
    if (ARM7_read_word(0x300FFFC) != SHWRAM_END2) fail_test(0x020);


    // switch to mod 3: ARM7 gets everything
    REG_WRAMCNT = 3;

    // check WRAMSTAT
    if ((ARM7_read_byte(0x4000241) & 3) != 3) fail_test(0x021);

    // ensure ARM7 can read it from anywhere
    if (ARM7_read_word(0x3000000) != SHWRAM_START1) fail_test(0x022);
    if (ARM7_read_word(0x3003FFC) != SHWRAM_END1) fail_test(0x023);
    if (ARM7_read_word(0x300C000) != SHWRAM_START2) fail_test(0x024);
    if (ARM7_read_word(0x300FFFC) != SHWRAM_END2) fail_test(0x025);
    // + from a mirror
    if (ARM7_read_word(0x3110000) != SHWRAM_START1) fail_test(0x026);
    if (ARM7_read_word(0x3223FFC) != SHWRAM_END1) fail_test(0x027);
    if (ARM7_read_word(0x333C000) != SHWRAM_START2) fail_test(0x028);
    if (ARM7_read_word(0x344FFFC) != SHWRAM_END2) fail_test(0x029);


    // tell ARM7 we're done
    ARM7_end_rw();
}
