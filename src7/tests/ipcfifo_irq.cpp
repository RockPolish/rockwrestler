// tests IPCFIFO interrupts
#include "../inc.h"

volatile int magic;

static void irqhandler()
{
    REG_IF = 1 << 17; // acknowledge interrupt
    magic = 89;
}

void arm7_ipcfifo_irq_test()
{
    // first, wait a bit
    wait_busy(65536);

    // now send word with IPCFIFO: ARM7 has "receive not empty" IRQ enabled
    REG_IPCFIFOCNT = (1 << 2) | (1 << 3) | (1 << 15); // "send fifo empty IRQ" + flush FIFO + enable
    // -> we'll wake up when ARM9 reads our message
    *IRQHANDLER = &irqhandler;
    REG_IF = 1 << 17;
    REG_IE = 1 << 17;
    REG_IME = 1;
    l_EIC();

    // initialize magic value before sending the value to avoid race conditions
    magic = 41;
    
    // send 0x8100
    REG_IPCFIFOSEND = 0x8100;

    // wait possibly a long time.. don't want to test tight synchronization
    int timeout = 65536 * 16;
    while (magic != 89)
    {
        if (--timeout == 0)
        {
            return;
        }
    }

    // need to tell ARM9 that our interrupt arrived
    REG_IPCFIFOCNT = (1 << 3) | (1 << 15);
    wait_busy(8192);

    REG_IPCFIFOSEND = 0xA50;
    REG_IME = 0;
}
