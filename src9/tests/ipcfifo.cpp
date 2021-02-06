// tests IPCFIFO (not entirely complete)
#include "../inc.h"

void run_tests_ipcfifo()
{
    REG_IPCFIFOCNT = 1 << 15; // enable FIFO
    REG_IPCSYNC = 12 << 8; // tell ARM7 to start IPCFIFO test

    // ARM7 starts by sending 0x12345678
    if (read_from_IPCFIFO(0x000) != 0x12345678) fail_test(0x001);

    // write 0 to IPCSYNC
    REG_IPCSYNC = 0;

    if (!(REG_IPCFIFOCNT & 1)) fail_test(0x002); // send empty = 1
    if (REG_IPCFIFOCNT & (1 << 1)) fail_test(0x003); // send full = 0
    if (!(REG_IPCFIFOCNT & (1 << 8))) fail_test(0x004); // receive empty = 1
    if (REG_IPCFIFOCNT & (1 << 9)) fail_test(0x005); // receive full = 0
    if (REG_IPCFIFOCNT & (1 << 14)) fail_test(0x006); // error = 0

    // send three words
    REG_IPCFIFOSEND = 0x1844CCAB;
    REG_IPCFIFOSEND = 0xB914A201;
    REG_IPCFIFOSEND = 0x1844CCAC;

    // wait until "receive full" bit is on
    wait_bit_value(0x007, IPCFIFOCNT, 9, true);

    if (!(REG_IPCFIFOCNT & 1)) fail_test(0x008); // send empty = 1
    if (REG_IPCFIFOCNT & (1 << 1)) fail_test(0x009); // send full = 0
    if (REG_IPCFIFOCNT & (1 << 8)) fail_test(0x00A); // receive empty = 0
    if (REG_IPCFIFOCNT & (1 << 14)) fail_test(0x00B); // error = 0

    // read 16 words, make sure they match
    for(int i = 0; i < 16; i++)
    {
        u32 word = REG_IPCFIFORECV;
        if (word != (0x11000000 + i)) fail_test(0x010 + i);
    }

    if (!(REG_IPCFIFOCNT & 1)) fail_test(0x020); // send empty = 1
    if (REG_IPCFIFOCNT & (1 << 1)) fail_test(0x021); // send full = 0
    if (!(REG_IPCFIFOCNT & (1 << 8))) fail_test(0x022); // receive empty = 1
    if (REG_IPCFIFOCNT & (1 << 9)) fail_test(0x023); // receive full = 0
    if (REG_IPCFIFOCNT & (1 << 14)) fail_test(0x024); // error = 0

    // read from empty queue: should read value that was last read, and set error bit
    u32 word = REG_IPCFIFORECV;
    if (word != 0x1100000F) fail_test(0x025); // seems to fail on desmume/melonDS, passes on no$gba
    
    if (!(REG_IPCFIFOCNT & 1)) fail_test(0x026); // send empty = 1
    if (REG_IPCFIFOCNT & (1 << 1)) fail_test(0x027); // send full = 0
    if (!(REG_IPCFIFOCNT & (1 << 8))) fail_test(0x028); // receive empty = 1
    if (REG_IPCFIFOCNT & (1 << 9)) fail_test(0x029); // receive full = 0
    if (!(REG_IPCFIFOCNT & (1 << 14))) fail_test(0x02A); // error = 1

    // write 0 to error bit, should NOT turn it off
    REG_IPCFIFOCNT = REG_IPCFIFOCNT & ~(1 << 14);
    
    if (!(REG_IPCFIFOCNT & (1 << 14))) fail_test(0x02B); // error = 1

    // acknowledge it by writing a 1
    REG_IPCFIFOCNT = REG_IPCFIFOCNT; // don't optimize this out

    if (REG_IPCFIFOCNT & (1 << 14)) fail_test(0x02C); // error = 0

    REG_IPCSYNC = 0;
}
