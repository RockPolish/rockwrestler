// tests IPCFIFO
#include "../inc.h"

void arm7_ipcfifo_test()
{
    // send 0x12345678
    REG_IPCFIFOSEND = 0x12345678;

    wait_busy(4096);

    // we want to have received 3 words
    // if any check here doesn't pass: reset ARM7. ARM9 will be waiting for a response, if it doesn't get it, it's a fail
    if (!(REG_IPCFIFOCNT & 1)) return; // send empty = 1
    if (REG_IPCFIFOCNT & (1 << 1)) return; // send full = 0
    if (REG_IPCFIFOCNT & (1 << 8)) return; // receive empty = 0
    if (REG_IPCFIFOCNT & (1 << 9)) return; // receive full = 0
    if (REG_IPCFIFOCNT & (1 << 14)) return; // error = 0

    u32 w1 = REG_IPCFIFORECV;
    u32 w2 = REG_IPCFIFORECV;
    u32 w3 = REG_IPCFIFORECV;

    if (!(REG_IPCFIFOCNT & 1)) return; // send empty = 1
    if (REG_IPCFIFOCNT & (1 << 1)) return; // send full = 0
    if (!(REG_IPCFIFOCNT & (1 << 8))) return; // receive empty = 1
    if (REG_IPCFIFOCNT & (1 << 9)) return; // receive full = 0
    if (REG_IPCFIFOCNT & (1 << 14)) return; // error = 0

    // + check to see if the words are correct
    if (w1 != 0x1844CCAB) return;
    if (w2 != 0xB914A201) return;
    if (w3 != 0x1844CCAC) return;

    // send 16 values so that the queue is full
    for(int i = 0; i < 16; i++)
    {
        REG_IPCFIFOSEND = 0x11000000 + i;
    }
}
