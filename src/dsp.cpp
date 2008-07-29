/*
	DSP M56001 emulation
	Dummy emulation, Aranym glue

	(C) 2001-2008 ARAnyM developer team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "sysdeps.h"
#include "hardware.h"
#include "cpu_emulation.h"
#include "memory.h"
#include "dsp.h"

#define DEBUG 0
#include "debug.h"

DSP::DSP(memptr address, uint32 size) : BASE_IO(address, size)
{
#if DSP_EMULATION
	dsp_core_init(&dsp_core);
#endif
}

DSP::~DSP(void)
{
#if DSP_EMULATION
	dsp_core_shutdown(&dsp_core);
#endif
}

/* Other functions to init/shutdown dsp emulation */
void DSP::reset(void)
{
#if DSP_EMULATION
	dsp_core_reset(&dsp_core);
#endif
}

/**********************************
 *	Hardware address read/write by CPU
 **********************************/

uint8 DSP::handleRead(memptr addr)
{
	uint8 value;
#if DSP_EMULATION
	value = dsp_core_read_host(&dsp_core, addr-getHWoffset());
#else
	/* this value prevents TOS from hanging in the DSP init code */
	value = 0xff;
#endif

	D(bug("HWget_b(0x%08x)=0x%02x at 0x%08x", addr, value, showPC()));
	return value;
}

void DSP::handleWrite(memptr addr, uint8 value)
{
	D(bug("HWput_b(0x%08x,0x%02x) at 0x%08x", addr, value, showPC()));
#if DSP_EMULATION
	dsp_core_write_host(&dsp_core, addr-getHWoffset(), value);
#endif
}

/*
vim:ts=4:sw=4:
*/
