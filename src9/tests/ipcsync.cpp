// tests IPCSYNC + interrupts
#include "../inc.h"

static volatile int magic;

static void irqhandler()
{
    // set magic to 123 if we received a 0
    REG_IF = 1 << 16; // acknowledge interrupt
    if ((REG_IPCSYNC & 0xF) == 0)
    {
        magic = 123;
    }
}

void wait_ipcsync_value(int value, int test_number)
{
    int timeout = 16384;
    while ((REG_IPCSYNC & 0xF) != value)
    {
        if (--timeout == 0)
        {
            timeout_test(test_number);
        }
    }
}

void run_tests_ipcsync()
{
    // send 13
    REG_IPCSYNC = 13 << 8;
    
    // wait to receive 4
    wait_ipcsync_value(4, 0x000);

    // send 5
    REG_IPCSYNC = 5 << 8;

    // receive 7
    wait_ipcsync_value(7, 0x001);

    // now with interrupts
    // "wait a bit"
    wait_busy(4096);

    // send 9 with interrupt
    REG_IPCSYNC = (9 << 8) | (1 << 13);

    // receive 11
    wait_ipcsync_value(11, 0x002);

    // now wait for ARM7 to send us interrupt
    magic = 77;
    REG_IPCSYNC = 1 << 14; // enable IRQ from remote CPU
    l_EIC();
    *IRQHANDLER = &irqhandler;
    REG_IF = 1 << 16; // clear bit in case it was still on
    REG_IE = 1 << 16; // enable IPCSYNC IRQ
    REG_IME = 1;

    int timeout = 65536;
    while (magic != 123)
    {
        if (--timeout == 0)
        {
            timeout_test(0x003);
        }
    }

    REG_IME = 0;
    REG_IPCSYNC = 0;
}
