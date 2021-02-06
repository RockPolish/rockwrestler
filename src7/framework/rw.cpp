// lets the ARM7 wait for requests by the ARM9 to read from an address + return the value, or write a value to an address
#include "../inc.h"

void arm7_rwmode()
{
    // enable FIFO
    REG_IPCFIFOCNT = 1 << 15;

    while (true)
    {
        if (REG_IPCFIFOCNT & (1 << 8)) continue; // only continue if queue not empty
        // read word: upper nibble says what to do, other 28 bits say which address
        u32 fifo_val = REG_IPCFIFORECV;
        int type = fifo_val >> 28;
        void* ptr = (void*)(fifo_val & 0x0FFFFFFF);
        if ((type >= 4) && (type <= 6))
        {
            // additionally wait for the value to be sent that we need to write
            // -> wait until receive empty bit is off
            while (REG_IPCFIFOCNT & (1 << 8)) { }
        }
        switch (type)
        {
            case 0:
            {
                // end rw mode: write a value as ARM9 is waiting for response
                REG_IPCFIFOSEND = 0;
                return;
            }
            case 1:
            {
                // read word
                REG_IPCFIFOSEND = *((vu32*)ptr);
                break;
            }
            case 2:
            {
                // read halfword
                REG_IPCFIFOSEND = *((vu16*)ptr);
                break;
            }
            case 3:
            {
                // read byte
                REG_IPCFIFOSEND = *((vu8*)ptr);
                break;
            }
            case 4:
            {
                // write word
                *((vu32*)ptr) = REG_IPCFIFORECV;
                REG_IPCFIFOSEND = 0;
                break;
            }
            case 5:
            {
                // write halfword
                *((vu16*)ptr) = REG_IPCFIFORECV;
                REG_IPCFIFOSEND = 0;
                break;
            }
            case 6:
            {
                // write byte
                *((vu8*)ptr) = REG_IPCFIFORECV;
                REG_IPCFIFOSEND = 0;
                break;
            }
            default:
            {
                // also end mode
                REG_IPCFIFOSEND = 0;
                return;
            }
        }
    }
}
