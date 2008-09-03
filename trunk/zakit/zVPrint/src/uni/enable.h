/*
 * @file uni/enable.h
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


#ifndef _ENABLE_H
#define _ENABLE_H


/*
 *  Prototypes
 */

BOOL APIENTRY OEMEnableDriver(
	DWORD dwOEMintfVersion,
	DWORD dwSize,
	PDRVENABLEDATA pded
);

VOID APIENTRY OEMDisableDriver();

PDEVOEM APIENTRY OEMEnablePDEV(
    PDEVOBJ				pdevobj,
    PWSTR				pPrinterName,
	ULONG				cPatterns,
    HSURF			   *phsurfPatterns,
    ULONG				cjGdiInfo,
    GDIINFO            *pGdiInfo,
    ULONG				cjDevInfo,
    DEVINFO            *pDevInfo,
    DRVENABLEDATA	   *pded,
	IPrintOemDriverUni *pKTHelp
);

VOID APIENTRY OEMDisablePDEV(
    PDEVOBJ pdevobj
);

BOOL APIENTRY OEMResetPDEV(
    PDEVOBJ pdevobjOld,
    PDEVOBJ pdevobjNew
);


#endif // !_ENABLE_H
