/*
 * @file uni/oemuni.h
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

#ifndef _OEMUNI_H
#define _OEMUNI_H

#include <PRCOMOEM.H>


/*
 *  Defines
 *

/*
 * Warning: the following enum order must match the 
 *          order in KTHookFuncs[].
 */
typedef enum tag_Hooks {
//    UD_DrvRealizeBrush,
//    UD_DrvDitherColor,
//    UD_DrvCopyBits,
    UD_DrvBitBlt,
    UD_DrvStretchBlt,
//    UD_DrvStretchBltROP,
//    UD_DrvPlgBlt,
//    UD_DrvTransparentBlt,
//    UD_DrvAlphaBlend,
//    UD_DrvGradientFill,
    UD_DrvTextOut,
    UD_DrvStrokePath,
//    UD_DrvFillPath,
//    UD_DrvStrokeAndFillPath,
//    UD_DrvPaint,
//    UD_DrvLineTo,
    UD_DrvStartPage,
    UD_DrvSendPage,
//    UD_DrvEscape,
    UD_DrvStartDoc,
    UD_DrvEndDoc,
    UD_DrvNextBand,
    UD_DrvStartBanding,
//    UD_DrvQueryFont,
//    UD_DrvQueryFontTree,
//    UD_DrvQueryFontData,
//    UD_DrvQueryAdvanceWidths,
//    UD_DrvFontManagement,
//    UD_DrvGetGlyphMode,

    MAX_DDI_HOOKS,

} ENUMHOOKS;


/*
 *  Type definitions
 */

// scanline pointer
typedef struct _scanlineptr
{
	PBYTE	pBits;		// scanline bits
	BOOL	bFlag;		// allocate flag

} SCANLINEPTR, *PSCANLINEPTR;

// DIB information
typedef struct _dibinfo
{
	PSCANLINEPTR pScans;				// scanline pointer
	BOOL		 bPageStarted;			// page started?
	SIZE		 DIBSize;				// DIB size
	DWORD		 WidthBytes,			// DIB width bytes
				 dwDIBTotalBytes;		// DIB total size
	LONG		 XPelsPerMeter,			// horizontal resolution, in pixels-per-meter
				 YPelsPerMeter;			// vertical resolution, in pixels-per-meter

} DIBINFO, *PDIBINFO;

typedef struct _oempdev {
    // This DLL hooks out every drawing DDI. So it needs to remember
    // Unidrv's hook function pointer so it call back.
    PFN					pfnUnidrv[MAX_DDI_HOOKS];

    // define whatever needed, such as working buffers, tracking information, etc

	IPrintOemDriverUni *pOEMHelp;	// Unidrv helper pointer
	HANDLE				hHeap;		// heap handle used by printing procedure

    TCHAR   szPreviewFileName[MAX_PATH]; // preview file name
    BOOL    bAppendPageNum;         // append page number automatically

    short   PageNum;                // page number
	LONG	approHeight;	        // approximate height
	DIBINFO DIBInfo;		        // dump DIB information

} OEMPDEV, *POEMPDEV;


#endif // !_OEMUNI_H
