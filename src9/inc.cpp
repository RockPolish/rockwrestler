#include "inc.h"

u32 read_from_IPCFIFO(int test_number)
{
    // wait until bit 8 (receive empty) is off
    wait_bit_value(test_number, IPCFIFOCNT, 8, false);
    // then read one value
    return REG_IPCFIFORECV;
}

extern "C" void init_iostate()
{
    // set CP15 control register: disables PU, enables ITCM/DTCM
    // also disables data (bit 2) + instruction (bit 12) cache
    cp15_set_controlreg(0x00052078);

    REG_POWCNT1 = 0x8003; // enable both LCDs, 2D engine A, + display swap
    REG_VRAMCNT_A = 0x80; // bank A in LCDC mode
    REG_DISPCNT = 0x20000; // VRAM display, block A

    // set DTCM location, + ITCM
    cp15_set_DTCMcontrol(0x0080000A);
    cp15_set_ITCMcontrol(0x0000000C);

    // disable IME
    REG_IME = 0;
}

void wait_vblank()
{
    // wait for V-blank flag to be unset, then wait for it to be set
    while ((REG_DISPSTAT & 1) == 1) { }
    while ((REG_DISPSTAT & 1) == 0) { }
}

// leftover from calling this label in assembly
extern "C" void l_resetIPCSYNC()
{
    REG_IPCSYNC = 0;
}

// fill screen with light gray color
void clear_screen()
{
    for(int i = 0; i < 256*192/2; i++)
    {
        ((u32*)0x6800000)[i] = 0x56B556B5;
    }
}

// clear one 8x8 tile with a color
// 0 <= x < 32, 0 <= y < 24
void draw_cleartile(int x, int y, u16 col)
{
    for(int j = 0; j < 8; j++)
    {
        for(int i = 0; i < 8; i++)
        {
            ((u16*)0x6800000)[(i + 8*x) + 256*(j + 8*y)] = col;
        }
    }
}

const u16 palette[] = {
    0x0000, // (0, 0, 0)
    0x294A, // (10, 10, 10)
    0x56B5, // (21, 21, 21)
    0x7FFF  // (31, 31, 31)
};
extern u8 font_start[]; // all tiles

// draw one tile
// 0 <= x < 32, 0 <= y < 24
void draw_tile(int x, int y, int tile)
{
    u8* tile_start = &font_start[64*tile];

    for(int j = 0; j < 8; j++)
    {
        for(int i = 0; i < 8; i++)
        {
            u8 palette_index = tile_start[i + 8*j];
            ((u16*)0x6800000)[(i + 8*x) + 256*(j + 8*y)] = palette[palette_index];
        }
    }
}

// draw a string (\n .. not supported)
// 0 <= x < 32, 0 <= y < 24
void draw_string(int x, int y, const char* str)
{
    while (*str)
    {
        char c = *(str++);
        draw_tile(x++, y, c);
    }
}
