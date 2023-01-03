// tests DTCM/ITCM
#include "../inc.h"

void run_tests_tcm()
{
    u32 cp15control = cp15_get_controlreg();
    cp15control |= 1 << 16; // enable DTCM
    cp15control &= ~(1 << 17); // disable DTCM load mode
    cp15_set_controlreg(cp15control);

    // put DTCM at 00 80 00 00, virtual size = 5 (512 bytes x 2^5 -> 16 KB)
    cp15_set_DTCMcontrol(0x0080000A);
    u32 masked_readback = cp15_get_DTCMcontrol() & 0xFFFFF03E;  // only check relevant bits: 12-31 for region base, 1-5 for virtual size
    if (masked_readback != 0x0080000A) fail_test(0x000);

    // write magic value to start/end
    *((vu32*)0x00800000) = DTCM_START;
    *((vu32*)0x00803FFC) = DTCM_END;

    // try reading those
    if (*((vu32*)0x00800000) != DTCM_START) fail_test(0x001);
    if (*((vu32*)0x00803FFC) != DTCM_END) fail_test(0x002);

    // move DTCM to 00 60 00 00, set virtual size to 6 (32 KB, so mirrored!)
    cp15_set_DTCMcontrol(0x0060000C);

    // read start + mirrored start + end + mirrored end
    if (*((vu32*)0x00600000) != DTCM_START) fail_test(0x003);
    if (*((vu32*)0x00604000) != DTCM_START) fail_test(0x004);
    if (*((vu32*)0x00603FFC) != DTCM_END) fail_test(0x005);
    if (*((vu32*)0x00607FFC) != DTCM_END) fail_test(0x006);

    // move DTCM to 03 00 00 00 (shared WRAM) after writing magic values there
    REG_WRAMCNT = 0; // allocate all to ARM9

    *((vu32*)0x03000000) = SHWRAM_START1;
    *((vu32*)0x03007FFC) = SHWRAM_END2;

    cp15_set_DTCMcontrol(0x0300001A); // base: 0x03000000, virtual size 13 (-> 4 MB)
    // DTCM overlaps with WRAM: DTCM takes priority
    if (*((vu32*)0x03000000) != DTCM_START) fail_test(0x007);
    if (*((vu32*)0x033FFFFC) != DTCM_END) fail_test(0x008); // + 4 MB

    // now set virtual size to 4 (8 KB) -> read DTCM value from start, WRAM value from end
    cp15_set_DTCMcontrol(0x03000008);
    if (*((vu32*)0x03000000) != DTCM_START) fail_test(0x009);
    if (*((vu32*)0x03007FFC) != SHWRAM_END2) fail_test(0x00A);



    // enable DTCM load mode: reading should give WRAM values, while writing writes to DTCM
    cp15control = cp15_get_controlreg();
    cp15control |= 1 << 17; // enable DTCM load mode
    cp15_set_controlreg(cp15control);

    // + virtual size to 32 KB, base at 0x03000000 still
    cp15_set_DTCMcontrol(0x0300000C);

    // read WRAM value from start/end
    if (*((vu32*)0x03000000) != SHWRAM_START1) fail_test(0x010);
    if (*((vu32*)0x03007FFC) != SHWRAM_END2) fail_test(0x011);

    // write new DTCM values
    *((vu32*)0x03000000) = DTCM_START + 1;
    *((vu32*)0x03007FFC) = DTCM_END + 1;

    // move DTCM back to 00 80 00 00 without load mode, read new DTCM values from there, and read old SHWRAM start
    cp15control = cp15_get_controlreg();
    cp15control &= ~(1 << 17); // disable DTCM load mode
    cp15_set_controlreg(cp15control);
    cp15_set_DTCMcontrol(0x0080000A); // 0x00800000, 16 KB

    if (*((vu32*)0x00800000) != (DTCM_START + 1)) fail_test(0x012);
    if (*((vu32*)0x00803FFC) != (DTCM_END + 1)) fail_test(0x013);

    if (*((vu32*)0x03000000) != SHWRAM_START1) fail_test(0x014); // make sure load-mod write did not touch this value

    // ITCM: 32 KB, always locked at 0x00000000, but virtual size can be changed
    cp15control = cp15_get_controlreg();
    cp15control |= 1 << 18; // enable ITCM
    cp15control &= ~(1 << 19); // disable ITCM load mode
    cp15_set_controlreg(cp15control);
    cp15_set_ITCMcontrol(0xC); // virtual size: 32 KB
    masked_readback = cp15_get_ITCMcontrol() & 0xFFFFF03E;  // only check relevant bits: 12-31 for region base, 1-5 for virtual size
    if (masked_readback != 0xC) fail_test(0x020);

    *((vu32*)0x00000000) = ITCM_START;
    *((vu32*)0x00007FFC) = ITCM_END;

    // try reading
    if (*((vu32*)0x00000000) != ITCM_START) fail_test(0x021);
    if (*((vu32*)0x00007FFC) != ITCM_END) fail_test(0x022);

    // increase virtual size, read mirrors
    cp15_set_ITCMcontrol(0xE); // 64 KB
    if (*((vu32*)0x00008000) != ITCM_START) fail_test(0x023);
    if (*((vu32*)0x0000FFFC) != ITCM_END) fail_test(0x024);

    // TODO. ITCM load mode, let ITCM and DTCM overlap, let ITCM and other memory section overlap, ...

        
    // restore DTCM state, as other tests (with interrupts) rely on DTCM location being 00800000
    cp15_set_DTCMcontrol(0x0080000A);
}
