/* 2001 MJ */

/*
 *  cpu_emulation.h - Definitions for ARAnyM CPU emulation module
 *
 *  based on
 *
 *  Basilisk II (C) 1997-2001 Christian Bauer
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef CPU_EMULATION_H
#define CPU_EMULATION_H

/*
 *  Memory system
 */

#include "sysdeps.h"
#include "memory.h"
#include "natfeats.h"

// RAM and ROM pointers (allocated and set by main_*.cpp)
// This part will be moved to another include, only mains use it.
extern memptr RAMBase;		// RAM base (Atari address space), does not include Low Mem when != 0
extern uint8 *RAMBaseHost;	// RAM base (host address space)
extern uint32 RAMSize;		// Size of RAM
extern memptr ROMBase;		// ROM base (Atari address space)
extern uint8 *ROMBaseHost;	// ROM base (host address space)
extern uint32 ROMSize;		// Size of ROM
extern uint32 RealROMSize;	// Real size of ROM
extern memptr HWBase;		// HW base (Atari address space)
extern uint8 *HWBaseHost;	// HW base (host address space)
extern uint32 HWSize;		// Size of HW space

extern memptr FastRAMBase;	// Fast-RAM base (Atari address space)
extern uint8 *FastRAMBaseHost;	// Fast-RAM base (host address space)
extern memptr VideoRAMBase;	// VideoRAM base (Atari address space)
extern uint8 *VideoRAMBaseHost;	// VideoRAM base (host address space)

#ifdef HW_SIGSEGV
extern uint8 *FakeIOBaseHost;
#endif

#ifdef RAMENDNEEDED
# define RAMEnd 0x01000000	// Not accessible top of memory
#else
# define RAMEnd 0
#endif

// Atari memory access functions
// Direct access to CPU address space
// For HW operations
// Read/WriteAtariIntXX
//
static inline uint32 ReadAtariInt32(memptr addr) {return phys_get_long(addr);}
static inline uint16 ReadAtariInt16(memptr addr) {return phys_get_word(addr);}
static inline uint8 ReadAtariInt8(memptr addr) {return phys_get_byte(addr);}
static inline void WriteAtariInt32(memptr addr, uint32 l) {phys_put_long(addr, l);}
static inline void WriteAtariInt16(memptr addr, uint16 w) {phys_put_word(addr, w);}
static inline void WriteAtariInt8(memptr addr, uint8 b) {phys_put_byte(addr, b);}

// Direct access to allocated memory
// Ignores HW checks, so that be carefull
// Read/WriteHWMemIntXX
// 
static inline uint32 ReadHWMemInt32(memptr addr) {return do_get_mem_long((uae_u32 *)phys_get_real_address(addr));}
static inline uint16 ReadHWMemInt16(memptr addr) {return do_get_mem_word((uae_u16 *)phys_get_real_address(addr));}
static inline uint8 ReadHWMemInt8(memptr addr) {return do_get_mem_byte((uae_u8 *)phys_get_real_address(addr));}
static inline void WriteHWMemInt32(memptr addr, uint32 l) {do_put_mem_long((uae_u32 *)phys_get_real_address(addr), l);}
static inline void WriteHWMemInt16(memptr addr, uint16 w) {do_put_mem_word((uae_u16 *)phys_get_real_address(addr), w);}
static inline void WriteHWMemInt8(memptr addr, uint8 b) {do_put_mem_byte((uae_u8 *)phys_get_real_address(addr), b);}

// Indirect access to CPU address space
// Uses MMU if available
// For SW operations
// Only data space
// Read/WriteIntXX
// 
static inline uint32 ReadInt32(memptr addr) {return get_long(addr);}
static inline uint16 ReadInt16(memptr addr) {return get_word(addr);}
static inline uint8 ReadInt8(memptr addr) {return get_byte(addr);}
static inline void WriteInt32(memptr addr, uint32 l) {put_long(addr, l);}
static inline void WriteInt16(memptr addr, uint16 w) {put_word(addr, w);}
static inline void WriteInt8(memptr addr, uint8 b) {put_byte(addr, b);}

// For Exception longjmp
extern jmp_buf excep_env;

// For address validation
static inline bool ValidAtariAddr(memptr addr, bool write, uint32 len) { return phys_valid_address(addr, write, 0, len); }
static inline bool ValidAddr(memptr addr, bool write, uint32 len) { return valid_address(addr, write, 0, len); }

// This function will be removed
static inline uint8 *Atari2HostAddr(memptr addr) {return phys_get_real_address(addr);}


// These functions will be removed
static inline void *Atari_memset(memptr addr, int c, size_t n) {return memset(Atari2HostAddr(addr), c, n);}
static inline void *Atari2Host_memcpy(void *dest, memptr src, size_t n) {return memcpy(dest, Atari2HostAddr(src), n);}
static inline void *Host2Atari_memcpy(memptr dest, const void *src, size_t n) {return memcpy(Atari2HostAddr(dest), src, n);}
static inline void *Atari2Atari_memcpy(memptr dest, memptr src, size_t n) {return memcpy(Atari2HostAddr(dest), Atari2HostAddr(src), n);}

/*
 *  680x0 emulation
 */

// Initialization
extern bool InitMEM();
extern bool Init680x0(void);
extern void Reset680x0(void);
extern void Exit680x0(void);
extern void AtariReset(void);

// 680x0 emulation functions
struct M68kRegisters;
extern void Start680x0(void);	// Reset and start 680x0
extern void Restart680x0(void);	// Restart running 680x0
extern void Quit680x0(void);	// Quit 680x0

// Interrupt functions
extern void TriggerInternalIRQ(void);
extern void TriggerInt3(void);		// Trigger interrupt level 3
extern void TriggerVBL(void);		// Trigger interrupt level 4
extern void TriggerInt5(void);		// Trigger interrupt level 5
extern void TriggerMFP(bool);		// Trigger interrupt level 6
extern void TriggerNMI(void);		// Trigger interrupt level 7

// This function will be removed
static uaecptr showPC(void) { return m68k_getpc(); }	// for debugging only

#endif
