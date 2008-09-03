/*
 * @file lib/devmode.h
 *
 * W2kPrintDrvSample
 *
 * Copyright (C) 2004, terrificskyfox <terrificskyfox@yahoo.com.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _DEVMODE_H
#define _DEVMODE_H


/*
 *  Defines
 */

#define NUM_COLORS	    	7 // number of supported colors

#define PCOLOR_BLACK		0
#define PCOLOR_RED			1
#define PCOLOR_GREEN		2
#define PCOLOR_BLUE			3
#define PCOLOR_YELLOW		4
#define PCOLOR_MAGENTA		5
#define PCOLOR_CYAN			6
#define PCOLOR_NONE			7


/*
 *  Type definitions
 */

typedef struct _peninfo
{
    BOOL pibBitmaped,
         pibPath,
         pibText;

} PENINFO, *PPENINFO;

typedef struct _oemdev
{
    OEM_DMEXTRAHEADER   dmKTExtra;

    PENINFO dmPenInfo[NUM_COLORS];	// pen information

    // for internal use
    short   dmOldColor;

} OEMDEV, *POEMDEV;


/*
 *  Prototypes
 */

HRESULT hrOEMDevMode(DWORD dwMode, POEMDMPARAM pOemDMParam);


#endif // !_DEVMODE_H
