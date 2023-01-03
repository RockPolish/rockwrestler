#pragma once
#include <stdint.h>

// types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef void (*voidfncptr)();

// asm functions
extern "C" void wait_busy(int steps);
extern "C" void l_EIC();
extern "C" void l_DIC();
extern "C" void call_fncptr(voidfncptr);

// C++ functions called by asm
extern "C" void enable_trace();
extern "C" void disable_trace();

//// IO registers
// IPC
vu16* const IPCSYNC = (vu16*)0x4000180;
vu16* const IPCFIFOCNT = (vu16*)0x4000184;
vu32* const IPCFIFOSEND = (vu32*)0x4000188;
vu32* const IPCFIFORECV = (vu32*)0x4100000;

#define REG_IPCSYNC (*IPCSYNC)
#define REG_IPCFIFOCNT (*IPCFIFOCNT)
#define REG_IPCFIFOSEND (*IPCFIFOSEND)
#define REG_IPCFIFORECV (*IPCFIFORECV)

// interrupts
vu32* const IME = (vu32*)0x4000208;
vu32* const IE = (vu32*)0x4000210;
vu32* const IF = (vu32*)0x4000214;

#define REG_IME (*IME)
#define REG_IE (*IE)
#define REG_IF (*IF)



// memory region markers
const u32 DTCM_START = 0x12345678;
const u32 DTCM_END = 0x87654321;

const u32 ITCM_START = 0x5A5B5C5D;
const u32 ITCM_END = 0x6A6B6C6D;

// shared WRAM (32 KB): two banks of 16
const u32 SHWRAM_START1 = 0x1A2B3C4D;
const u32 SHWRAM_END1 = 0x1F2F3F4F;
const u32 SHWRAM_START2 = 0x2F3F4F5F;
const u32 SHWRAM_END2 = 0x89ABCDEF;

const u32 VRAM_A_START = 0xA1A2A3A4;
const u32 VRAM_A_END = 0xA5A6A7A8;
const u32 VRAM_B_START = 0xB1B2B3B4;
const u32 VRAM_B_END = 0xB5B6B7B8;
const u32 VRAM_C_START = 0xC1C2C3C4;
const u32 VRAM_C_END = 0xC5C6C7C8;
const u32 VRAM_D_START = 0xD1D2D3D4;
const u32 VRAM_D_END = 0xD5D6D7D8;
const u32 VRAM_E_START = 0xE1E2E3E4;
const u32 VRAM_E_END = 0xE5E6E7E8;
const u32 VRAM_F_START = 0xF1F2F3F4;
const u32 VRAM_F_END = 0xF5F6F7F8;
const u32 VRAM_G_START = 0xA0A2A3A4;
const u32 VRAM_G_END = 0xA0A6A7A8;
const u32 VRAM_H_START = 0xB0A2A3A4;
const u32 VRAM_H_END = 0xB0A6A7A8;
const u32 VRAM_I_START = 0xC0A2A3A4;
const u32 VRAM_I_END = 0xC0A6A7A8;

const u32 ARM7RAM_START = 0xC0DE5890;
const u32 ARM7RAM_END = 0xDEADBEEF;
