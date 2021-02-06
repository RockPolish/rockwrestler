// ARM7 rw mode: can then do ARM7_read_word(address) to receive the value at that address on the ARM7 side
#include "../inc.h"

// this function pretty much does everything: reading, writing, ...
// do not call it directly, use the other 8 functions defined in this file
u32 ARM7_rw_core(u32 address, int type, u32 value)
{
    REG_IPCFIFOSEND = address | (type << 28); // transmit address + type in one go
    if ((type >= 4) && (type <= 6)) REG_IPCFIFOSEND = value; // also send value
    // wait for receive empty bit to be turned off
    int timeout = 8192;
    while (REG_IPCFIFOCNT & (1 << 8))
    {
        if (--timeout == 0)
        {
            timeout_rw();
        }
    }
    // return value we received (even when we write something: a value is returned to signal that the ARM7 has finished the write)
    return REG_IPCFIFORECV;
}

void ARM7_start_rw()
{
    // clear IPCFIFO queue
    REG_IPCFIFOCNT = (1 << 3) | (1 << 15); // send fifo clear + enable send/receive fifo
    // send 10 with IPCSYNC to tell ARM7 to await our read/write commands
    REG_IPCSYNC = 10 << 8;

    // wait a bit
    wait_busy(4096);

    // send 0 with IPCSYNC so ARM7 doesn't immediately go back to read/write mode after we end the test
    REG_IPCSYNC = 0;
}

u32 ARM7_read_word(u32 address)
{
    return ARM7_rw_core(address, 1, 0);
}

u16 ARM7_read_halfword(u32 address)
{
    return ARM7_rw_core(address, 2, 0);
}

u8 ARM7_read_byte(u32 address)
{
    return ARM7_rw_core(address, 3, 0);
}

void ARM7_write_word(u32 address, u32 value)
{
    ARM7_rw_core(address, 4, value);
}

void ARM7_write_halfword(u32 address, u16 value)
{
    ARM7_rw_core(address, 5, value);
}

void ARM7_write_byte(u32 address, u8 value)
{
    ARM7_rw_core(address, 6, value);
}

void ARM7_end_rw()
{
    ARM7_rw_core(0, 0, 0);
}
