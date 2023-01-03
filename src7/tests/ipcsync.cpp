// tests IPCSYNC + interrupts
#include "../inc.h"

static volatile int magic;

static void irqhandler()
{
    // set magic to 123 if we received a 0
    REG_IF = 1 << 16; // acknowledge interrupt
    if ((REG_IPCSYNC & 0xF) != 9) return;
    
    magic = 0x69;
    REG_IPCSYNC = 11 << 8; // send 11
    // wait a bit
    wait_busy(8192);

    // send 0 with IRQ
    REG_IPCSYNC = 1 << 13;
}

bool wait_ipcsync_value(int value)
{
    int timeout = 4096;
    while ((REG_IPCSYNC & 0xF) != value)
    {
        if (--timeout == 0)
        {
            return false;
        }
    }
    return true;
}

void arm7_ipcsync_test()
{
    // send 4
    REG_IPCSYNC = 4 << 8;

    // wait for 5
    if (!wait_ipcsync_value(5)) return;

    magic = 11;
    // send 7, enable IRQ
    *IRQHANDLER = &irqhandler;
    REG_IF = 1 << 16;
    REG_IE = 1 << 16;
    REG_IME = 1;
    l_EIC();
    REG_IPCSYNC = (7 << 8) | (1 << 14);

    int timeout = 65536;
    while (magic != 0x69)
    {
        if (--timeout == 0)
        {
            return;
        }
    }

    REG_IME = 0;
}
