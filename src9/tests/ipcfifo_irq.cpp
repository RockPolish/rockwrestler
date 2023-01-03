// tests IPCFIFO interrupts
#include "../inc.h"

static volatile int magic;

static void irqhandler()
{
    // set magic to 13 if we received 0x8100
    REG_IF = 1 << 18; // acknowledge interrupt
    if (REG_IPCFIFORECV == 0x8100)
    {
        magic = 13;
    }
}


void run_tests_ipcfifo_irq()
{
    // enable receive not empty IRQ in IE (IE.18) + acknowledge it first (IF)
    REG_IF = 1 << 18;
    REG_IE = 1 << 18;

    // enable "receive not empty" in IPCFIFOCNT
    REG_IPCFIFOCNT = (1 << 10) | (1 << 15); // receive fifo not empty IRQ + enable fifo

    // set IRQ handler
    *IRQHANDLER = &irqhandler;

    // enable interrupts (IME and CPSR)
    REG_IME = 1;
    l_EIC();

    // send 11 with IPCSYNC: this starts the test on ARM7 side, it will send to FIFO -> interrupt on our side
    magic = 88;
    REG_IPCSYNC = 11 << 8;

    int timeout = 65536*4;
    while (magic != 13) // IRQ handler changes magic to 13 if it read 0x8100 from FIFO
    {
        if (--timeout == 0)
        {
            timeout_test(0x000);
        }
    }

    REG_IPCSYNC = 0; // reset IPCSYNC

    REG_IME = 0; // disable interrupts
    // we read a word from the queue in our interrupt handler: ARM7 gets interrupt, will send 0xA50
    u32 read_from_fifo = read_from_IPCFIFO(0x001);

    if (read_from_fifo != 0xA50) fail_test(0x002);
}
