#include "../inc.h"
#include "../tests/tests.h"

// return from this function after a test: resets ARM7, just goes back into this loop but with stack, ... reset
extern "C" void arm7_waitloop()
{
    wait_busy(1 << 18); // wait a little bit so that ARM9 has definitely set IPCSYNC to 0
    REG_IPCSYNC = 0;
    REG_IPCFIFOCNT = 1 << 15; // enable FIFO
    
    // keep polling IPCSYNC to see if ARM9 wants us to start a test
    while (true)
    {
        int ipcsync_val = REG_IPCSYNC & 0xF;
        switch (ipcsync_val)
        {
            // heartbeat
            case 1:
            {
                REG_IPCSYNC = 1 << 8;
                break;
            }
            case 2:
            {
                REG_IPCSYNC = 2 << 8;
                break;
            }

            // rw mode
            case 10:
            {
                arm7_rwmode();
                return;
            }

            // IPC tests
            case 11:
            {
                arm7_ipcfifo_irq_test();
                return;
            }

            case 12:
            {
                arm7_ipcfifo_test();
                return;
            }

            case 13:
            {
                arm7_ipcsync_test();
                return;
            }
            default: break;
        }
    }
}
