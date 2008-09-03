/*
 * @file ui/oemui.h
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

#ifndef _OEMUI_H
#define _OEMUI_H

#include "devmode.h"


/*
 *  Type definitions
 */

typedef struct _dialoginfo
{
	PDEVMODE	pPubDev;		// public devmode
	POEMDEV		pOemDev;		// oem devmode
	POEMDEV		pOemDevOrig;	// original oem devmode
	HANDLE		hHeap;			// heap handle
	BOOL		bOK;

} DIALOGINFO, *PDIALOGINFO;


/*
 *  Prototypes
 */

HRESULT hrOEMCommonUIProp(DWORD dwMode, POEMCUIPPARAM pOemCUIPParam);
HRESULT hrOEMDevicePropertySheets(PPROPSHEETUI_INFO pPSUIInfo, LPARAM lParam);
HRESULT hrOEMDocumentPropertySheets(PPROPSHEETUI_INFO pPSUIInfo, LPARAM lParam);


#endif // !_OEMUI_H
