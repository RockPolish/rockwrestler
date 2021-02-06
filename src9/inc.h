#pragma once
#include "../common/common.h"

// asm functions
extern "C" void fail_test(int test_number);
extern "C" void timeout_test(int test_number);
extern "C" void timeout_rw();

extern "C" u32 cp15_get_controlreg();
extern "C" void cp15_set_controlreg(u32 value);
extern "C" u32 cp15_get_DTCMcontrol();
extern "C" void cp15_set_DTCMcontrol(u32 value);
extern "C" u32 cp15_get_ITCMcontrol();
extern "C" void cp15_set_ITCMcontrol(u32 value);

// C++ functions called from assembly
extern "C" void l_resetIPCSYNC();
extern "C" void init_iostate();

// C++ functions
void wait_vblank();

// drawing
void clear_screen();
void draw_cleartile(int x, int y, u16 col);
void draw_tile(int x, int y, int tile);
void draw_string(int x, int y, const char* str);

// rw
void ARM7_start_rw();
u32 ARM7_read_word(u32 address);
u16 ARM7_read_halfword(u32 address);
u8 ARM7_read_byte(u32 address);
void ARM7_write_word(u32 address, u32 value);
void ARM7_write_halfword(u32 address, u16 value);
void ARM7_write_byte(u32 address, u8 value);
void ARM7_end_rw();

u32 read_from_IPCFIFO(int test_number);

// wait for a certain bit to be set/unset in an IO register (or global var changed by interrupt)
template<class T>
void wait_bit_value(int test_number, volatile T* ptr, int bit_number, bool bit_value)
{
    int timeout = 16384;
    while (((*ptr >> bit_number) & 1) != bit_value)
    {
        if (--timeout == 0)
        {
            timeout_test(test_number);
        }
    }
}

// draw a given value in hexadecimal, only the lowest N digits
template<int N>
void draw_hex_value(int x, int y, u32 value)
{
    for(int i = 0; i < N; i++)
    {
        draw_tile(x+i, y, (value >> (4*(N-1-i))) & 0xF);
    }
}

// TODO add function to print in decimal


// non-IO registers
volatile voidfncptr* const IRQHANDLER = (voidfncptr*)0x00803FFC;

//// IO registers
vu32* const DISPCNT = (vu32*)0x4000000;
vu16* const DISPSTAT = (vu16*)0x4000004;

#define REG_DISPCNT (*DISPCNT)
#define REG_DISPSTAT (*DISPSTAT)

// keypad
vu16* const KEYINPUT = (vu16*)0x4000130;

#define REG_KEYINPUT (*KEYINPUT)
const u16 KEY_A = 0;
const u16 KEY_B = 1;
const u16 KEY_SELECT = 2;
const u16 KEY_START = 3;
const u16 KEY_RIGHT = 4;
const u16 KEY_LEFT = 5;
const u16 KEY_UP = 6;
const u16 KEY_DOWN = 7;
const u16 KEY_R = 8;
const u16 KEY_L = 9;

const u16 KEY_MASK_A = 1;
const u16 KEY_MASK_B = 2;
const u16 KEY_MASK_SELECT = 4;
const u16 KEY_MASK_START = 8;
const u16 KEY_MASK_RIGHT = 16;
const u16 KEY_MASK_LEFT = 32;
const u16 KEY_MASK_UP = 64;
const u16 KEY_MASK_DOWN = 128;
const u16 KEY_MASK_R = 256;
const u16 KEY_MASK_L = 512;

// memory control
vu8* const VRAMCNT_A = (vu8*)0x4000240;
vu8* const VRAMCNT_B = (vu8*)0x4000241;
vu8* const VRAMCNT_C = (vu8*)0x4000242;
vu8* const VRAMCNT_D = (vu8*)0x4000243;
vu8* const VRAMCNT_E = (vu8*)0x4000244;
vu8* const VRAMCNT_F = (vu8*)0x4000245;
vu8* const VRAMCNT_G = (vu8*)0x4000246;
vu8* const WRAMCNT = (vu8*)0x4000247;
vu8* const VRAMCNT_H = (vu8*)0x4000248;
vu8* const VRAMCNT_I = (vu8*)0x4000249;

#define REG_VRAMCNT_A (*VRAMCNT_A)
#define REG_VRAMCNT_B (*VRAMCNT_B)
#define REG_VRAMCNT_C (*VRAMCNT_C)
#define REG_VRAMCNT_D (*VRAMCNT_D)
#define REG_VRAMCNT_E (*VRAMCNT_E)
#define REG_VRAMCNT_F (*VRAMCNT_F)
#define REG_VRAMCNT_G (*VRAMCNT_G)
#define REG_WRAMCNT (*WRAMCNT)
#define REG_VRAMCNT_H (*VRAMCNT_H)
#define REG_VRAMCNT_I (*VRAMCNT_I)

// DIV
vu16* const DIVCNT = (vu16*)0x4000280;
vu32* const DIVNUMERLO = (vu32*)0x4000290;
vu32* const DIVNUMERHI = (vu32*)0x4000294;
vu32* const DIVDENOMLO = (vu32*)0x4000298;
vu32* const DIVDENOMHI = (vu32*)0x400029C;
vu32* const DIVRESLO = (vu32*)0x40002A0;
vu32* const DIVRESHI = (vu32*)0x40002A4;
vu32* const DIVREMLO = (vu32*)0x40002A8;
vu32* const DIVREMHI = (vu32*)0x40002AC;

#define REG_DIVCNT (*DIVCNT)
#define REG_DIVNUMERLO (*DIVNUMERLO)
#define REG_DIVNUMERHI (*DIVNUMERHI)
#define REG_DIVDENOMLO (*DIVDENOMLO)
#define REG_DIVDENOMHI (*DIVDENOMHI)
#define REG_DIVRESLO (*DIVRESLO)
#define REG_DIVRESHI (*DIVRESHI)
#define REG_DIVREMLO (*DIVREMLO)
#define REG_DIVREMHI (*DIVREMHI)

// SQRT
vu16* const SQRTCNT = (vu16*)0x40002B0;
vu32* const SQRTRES = (vu32*)0x40002B4;
vu32* const SQRTLO = (vu32*)0x40002B8;
vu32* const SQRTHI = (vu32*)0x40002BC;

#define REG_SQRTCNT (*SQRTCNT)
#define REG_SQRTRES (*SQRTRES)
#define REG_SQRTLO (*SQRTLO)
#define REG_SQRTHI (*SQRTHI)

// misc
vu32* const POWCNT1 = (vu32*)0x4000304;

#define REG_POWCNT1 (*POWCNT1)

