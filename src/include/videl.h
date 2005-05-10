/*
 * videl.h - Falcon VIDEL emulation - declaration
 *
 * Copyright (c) 2001-2004 ARAnyM developer team (see AUTHORS)
 *
 * Authors:
 *  joy		Petr Stehlik
 *	standa	Standa Opichal
 *	pmandin	Patrice Mandin
 * 
 * VIDEL HW regs inspired by Linux-m68k kernel (linux/drivers/video/atafb.c)
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
 */

#ifndef _VIDEL_H
#define _VIDEL_H

#include "icio.h"

class VIDEL : public BASE_IO {
protected:
	int width, height, bpp, since_last_change;
	bool hostColorsSync;
	bool doRender; // the HW surface is available -> videl writes directly into the Host videoram

	void renderScreenNoFlag();
	void renderScreenNoZoom();
	void renderScreenZoom();

	/* Autozoom */
	int zoomwidth, prev_scrwidth;
	int zoomheight, prev_scrheight;
	int *zoomxtable;
	int *zoomytable;

public:
	VIDEL(memptr, uint32);
	bool isMyHWRegister(memptr addr);
	void reset();

	virtual void handleWrite(uint32 addr, uint8 value);

	void updateColors();
	void renderScreen(void);
	void setRendering( bool render );

	int getScreenBpp();
	int getScreenWidth();
	int getScreenHeight();

	long getVideoramAddress();
};

inline void VIDEL::setRendering( bool render ) {
	doRender = render;
}

#endif /* _VIDEL_H */

/*
 * $Log$
 * Revision 1.24  2004/09/02 20:14:30  pmandin
 * Second fix for autozoom: recalculate zoom tables when host screen size changes
 *
 * Revision 1.23  2004/02/14 01:37:31  joy
 * reset all HW chips
 *
 * Revision 1.22  2003/11/25 22:56:49  joy
 * part of a major hardware dispatcher rewrite
 *
 * Revision 1.21  2003/06/01 08:35:39  milan
 * MacOS X support updated and <SDL/> removed from includes, path to SDL headers must be fully defined
 *
 * Revision 1.20  2003/04/16 19:35:49  pmandin
 * Correct inclusion of SDL headers
 *
 * Revision 1.19  2002/12/29 13:54:46  joy
 * linewidth and lineoffset registers emulated
 *
 * Revision 1.18  2002/09/24 16:08:24  pmandin
 * Bugfixes+preliminary autozoom support
 *
 * Revision 1.17  2002/04/22 18:30:50  milan
 * header files reform
 *
 * Revision 1.16  2002/02/28 20:44:22  joy
 * uae_ vars replaced with uint's
 *
 * Revision 1.15  2001/12/17 08:33:00  standa
 * Thread synchronization added. The check_event and fvdidriver actions are
 * synchronized each to other.
 *
 * Revision 1.14  2001/12/17 07:09:44  standa
 * SDL_delay() added to setRendering to avoid SDL access the graphics from
 * both VIDEL and fVDIDriver threads....
 *
 * Revision 1.13  2001/11/07 21:18:25  milan
 * SDL_CFLAGS in CXXFLAGS now.
 *
 * Revision 1.12  2001/11/04 23:17:08  standa
 * 8bit destination surface support in VIDEL. Blit routine optimalization.
 * Bugfix in compatibility modes palette copying.
 *
 * Revision 1.11  2001/08/28 23:26:09  standa
 * The fVDI driver update.
 * VIDEL got the doRender flag with setter setRendering().
 *       The host_colors_uptodate variable name was changed to hostColorsSync.
 * HostScreen got the doUpdate flag cleared upon initialization if the HWSURFACE
 *       was created.
 * fVDIDriver got first version of drawLine and fillArea (thanks to SDL_gfxPrimitives).
 *
 * Revision 1.10  2001/06/18 13:21:55  standa
 * Several template.cpp like comments were added.
 * HostScreen SDL encapsulation class.
 *
 * Revision 1.9  2001/06/17 21:56:32  joy
 * updated to reflect changes in .cpp counterpart
 *
 * Revision 1.8  2001/06/15 14:16:06  joy
 * VIDEL palette registers are now processed by the VIDEL object.
 *
 * Revision 1.7  2001/06/13 07:12:39  standa
 * Various methods renamed to conform the sementics.
 * Added videl fuctions needed for VDI driver.
 *
 *
 */

