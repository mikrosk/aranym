/*
 * disasm-x86.cpp - x86/x86_64 disassembler (using opcodes library)
 *
 * Copyright (c) 2017 ARAnyM developer team
 * 
 * This file is part of the ARAnyM project which builds a new and powerful
 * TOS/FreeMiNT compatible virtual machine running on almost any hardware.
 *
 * ARAnyM is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ARAnyM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ARAnyM; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * 2017-01-07 : Initial version - Thorsten Otto
 *
 */

#include "sysdeps.h"
#include "cpu_emulation.h"
#include "disasm-glue.h"

#ifdef HAVE_DISASM_X86 /* rest of file */

#include <dis-asm.h>

/*
 * newer versions of dis-asm.h don't declare the
 * functions :(
 */
#ifdef __cplusplus
extern "C" {
#endif
extern int print_insn_i386               (bfd_vma, disassemble_info *);
#ifdef __cplusplus
}
#endif

struct opcodes_info {
	char linebuf[128];
	size_t bufsize;
	size_t linepos;
	disassemble_info opcodes_info;
};


static int opcodes_printf(void *info, const char *format, ...)
{
	struct opcodes_info *opcodes_info = (struct opcodes_info *)info;
	va_list args;
	int len;
	size_t remain;
	
	va_start(args, format);
	remain = opcodes_info->bufsize - opcodes_info->linepos - 1;
	len = vsnprintf(opcodes_info->linebuf + opcodes_info->linepos, remain, format, args);
	if (len > 0)
	{
		if ((size_t)len > remain)
			len = remain;
		opcodes_info->linepos += len;
	}
	va_end(args);
	return len;
}


const uint8 *x86_disasm(const uint8 *ainstr, char *buf)
{
	struct opcodes_info info;
	int len;
	int i;
	char *opcode;
	char *p;
	
	info.linepos = 0;
	info.bufsize = sizeof(info.linebuf);
	INIT_DISASSEMBLE_INFO(info.opcodes_info, &info, opcodes_printf);
	info.opcodes_info.buffer = (bfd_byte *)ainstr;
	info.opcodes_info.buffer_length = 15; // largest instruction size on x86
	info.opcodes_info.buffer_vma = (uintptr)ainstr;
	info.opcodes_info.arch = bfd_arch_i386;
#ifdef CPU_i386
	info.opcodes_info.mach = bfd_mach_i386_i386;
#else
	info.opcodes_info.mach = bfd_mach_x86_64;
#endif
	disassemble_init_for_target(&info.opcodes_info);
	len = print_insn_i386(info.opcodes_info.buffer_vma, &info.opcodes_info);
	info.linebuf[info.linepos] = '\0';
#ifdef CPU_i386
	sprintf(buf, "[%08x]", (uintptr)ainstr);
#else
	sprintf(buf, "[%016lx]", (uintptr)ainstr);
#endif
	for (i = 0; i < 7 && i < len; i++)
	{
		sprintf(buf + strlen(buf), " %02x", ainstr[i]);
	}
	for (; i < 7; i++)
		strcat(buf, "   ");
	opcode = info.linebuf;
	if (strncmp(opcode, "addr32", 6) == 0)
	{
		opcode += 6;
		while (*opcode == ' ')
			opcode++;
	}		
	p = strchr(opcode, ' ');
	if (p)
	{
		*p++ = '\0';
		while (*p == ' ')
			p++;
	}
	sprintf(buf + strlen(buf), "  %-10s", opcode);
	if (p)
		strcat(buf, p);
	if (len <= 0) /* make sure we advance in case there was an error */
		len = 1;
	ainstr += len;
	return ainstr;
}

#else

extern int i_dont_care_that_ISOC_doesnt_like_empty_sourcefiles;

#endif /* HAVE_DISASM_X86 */
