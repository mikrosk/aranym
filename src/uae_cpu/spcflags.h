 /*
  * UAE - The Un*x Amiga Emulator
  *
  * MC68000 emulation
  *
  * Copyright 1995 Bernd Schmidt
  */

#ifndef SPCFLAGS_H
#define SPCFLAGS_H

typedef uae_u32 spcflags_t;

enum {
	SPCFLAG_STOP			= 0x01,
	SPCFLAG_INTERNAL_IRQ	= 0x02,
	SPCFLAG_BRK			= 0x04,
	SPCFLAG_TRACE			= 0x08,
	SPCFLAG_DOTRACE			= 0x10,
//	SPCFLAG_DOINT			= 0x20,
#ifdef USE_JIT
	SPCFLAG_JIT_END_COMPILE		= 0x40,
	SPCFLAG_JIT_EXEC_RETURN		= 0x80,
#else
	SPCFLAG_JIT_END_COMPILE		= 0,
	SPCFLAG_JIT_EXEC_RETURN		= 0,
#endif
	SPCFLAG_VBL			= 0x100,
	SPCFLAG_MFP			= 0x200,
	SPCFLAG_NMI			= 0x400,
	SPCFLAG_INT3		= 0x800,
	SPCFLAG_INT5		= 0x1000,
	SPCFLAG_MODE_CHANGE		= 0x4000,
	SPCFLAG_ALL			= SPCFLAG_STOP
					| SPCFLAG_INTERNAL_IRQ
					| SPCFLAG_BRK
					| SPCFLAG_TRACE
					| SPCFLAG_DOTRACE
//					| SPCFLAG_DOINT
					| SPCFLAG_JIT_END_COMPILE
					| SPCFLAG_JIT_EXEC_RETURN
					| SPCFLAG_INT3
					| SPCFLAG_VBL
					| SPCFLAG_INT5
					| SPCFLAG_MFP
					| SPCFLAG_NMI
					,
	
	SPCFLAG_ALL_BUT_EXEC_RETURN	= SPCFLAG_ALL & ~SPCFLAG_JIT_EXEC_RETURN

};

#define SPCFLAGS_TEST(m) \
	(regs.spcflags & (m))

/* Macro only used in m68k_reset() */
#define SPCFLAGS_INIT(m) do { \
	regs.spcflags = (m); \
} while (0)

#ifndef ENABLE_EXCLUSIVE_SPCFLAGS

#define SPCFLAGS_SET(m) do { \
	regs.spcflags |= (m); \
} while (0)

#define SPCFLAGS_CLEAR(m) do { \
	regs.spcflags &= ~(m); \
} while (0)

#elif defined(__i386__) && defined(X86_ASSEMBLY)

#define HAVE_HARDWARE_LOCKS 1

#define SPCFLAGS_SET(m) do { \
	__asm__ __volatile__("lock\n\torl %1,%0" : "=m" (regs.spcflags) : "i" ((m))); \
} while (0)

#define SPCFLAGS_CLEAR(m) do { \
	__asm__ __volatile__("lock\n\tandl %1,%0" : "=m" (regs.spcflags) : "i" (~(m))); \
} while (0)

#else

#undef HAVE_HARDWARE_LOCKS
extern  SDL_mutex *spcflags_lock;

#define SPCFLAGS_SET(m) do { 				\
	SDL_LockMutex(spcflags_lock);		\
	regs.spcflags |= (m);					\
	SDL_UnlockMutex(spcflags_lock);	\
} while (0)

#define SPCFLAGS_CLEAR(m) do {				\
	SDL_LockMutex(spcflags_lock);		\
	regs.spcflags &= ~(m);					\
	SDL_UnlockMutex(spcflags_lock);	\
} while (0)

#endif

#endif /* SPCFLAGS_H */
