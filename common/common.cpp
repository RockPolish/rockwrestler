#include "common.h"

// enable/disable trace on my emulator - disable_trace is called by the ARM7 after a test, by the ARM9 when failing a test
// it may be useful to enable the trace as a test starts and then look at the last few instructions before the test fails,
// as the trace is automatically disabled when failing
// these addresses are in the GBA cartridge range, writing to them does nothing

extern "C" void enable_trace()
{
    *((vu32*)0x8004400) = 0x8004400;
}

extern "C" void disable_trace()
{
    *((vu32*)0x8005500) = 0x8005500;
}
