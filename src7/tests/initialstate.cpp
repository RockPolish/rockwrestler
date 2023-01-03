#include "../inc.h"

extern u32 val_CPSR;

extern "C" void test_initialstate()
{
    l_DIC();
    // get some values of registers at boot time, store these at start of main RAM (4 MB) where ARM9 can read them freely
    vu32* result_base = (vu32*)0x2000004; // skip first word
    result_base[0] = REG_IPCSYNC;
    result_base[1] = REG_IPCFIFOCNT;
    // enable FIFO + clear error bit
    REG_IPCFIFOCNT = (1 << 15) | (1 << 14);
    // count how many values are in the queue (this also clears the queue)
    int FIFO_length = 0;
    while (!(REG_IPCFIFOCNT & (1 << 8))) // as long as the FIFO is not empty
    {
        vu32 FIFO_val = REG_IPCFIFORECV;
        FIFO_length++;
        if (FIFO_length > 20)
        {
            // IPCFIFO maybe unimplemented, shouldn't keep looping here forever
            FIFO_length = 0xFFFF;
            break;
        }
    }
    result_base[2] = FIFO_length;
    result_base[3] = REG_IME;
    result_base[4] = REG_IE;
    result_base[5] = REG_IF;
    REG_IME = 0;
    REG_IE = 0;
    REG_IF = 0xFFFFFFFF; // acknowledge all

    result_base[6] = val_CPSR;
}
