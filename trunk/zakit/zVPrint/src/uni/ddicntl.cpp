/*
 * @file uni/ddicntl.cpp DDI hook control function.
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

#include "precomp.h"
#include "debug.h"
#include "oemuni.h"
#include "devmode.h"
#include "dlgclbk.h"
#include "resource.h"
#include "output.h"
#include "ddicntl.h"


/*
 *  Internal implementations
 */

static void page_output(PDEVOBJ pdevobj)
{
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // convert to down-top DIB
	convert_to_normal_dib(pdevobj);

	// clean
	for(LONG Loop = 0; Loop < pOemPDEV->DIBInfo.DIBSize.cy; Loop++)
	{
		if(pOemPDEV->DIBInfo.pScans[Loop].bFlag)
			HeapFree(pOemPDEV->hHeap, 0, pOemPDEV->DIBInfo.pScans[Loop].pBits);
	}
	HeapFree(pOemPDEV->hHeap, 0, pOemPDEV->DIBInfo.pScans);

	pOemPDEV->DIBInfo.bPageStarted = FALSE;
    if(!pOemPDEV->bAppendPageNum)
        ZeroMemory(pOemPDEV->szPreviewFileName, MAX_PATH * sizeof(TCHAR));
}


/*
 *  Implementations
 */

/*
 * Function: OEMStartDoc
 *
 * Purpose: Called by GDI when it is ready to start sending a document
 *		    to the driver for rendering.
 *
 * Return Value: BOOL
 */
BOOL APIENTRY
OEMStartDoc(
    SURFOBJ    *pso,
    PWSTR       pwszDocName,
    DWORD       dwJobId)
{
	TERSE(UNITEXT("OEMStartDoc() entry.\r\n"));


	PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvStartDoc)(pOemPDEV->pfnUnidrv[UD_DrvStartDoc])) (
			pso,
			pwszDocName,
			dwJobId
			));
}

/*
 * Function: OEMStartPage
 *
 * Purpose: Called by GDI when it is ready to start sending the contents of
 *		    a physical page to the driver for rendering.
 *
 * Action: Initialize page-related environment, call back to Unidrv.
 *
 * Return Value: BOOL
 */
BOOL APIENTRY
OEMStartPage(
    SURFOBJ    *pso)
{
    TERSE(UNITEXT("OEMStartPage() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;
    PDEVMODE pPubDev = (PDEVMODE)pdevobj->pPublicDM;
	POEMDEV  pOemDev = (POEMDEV)pdevobj->pOEMDM;


    // increment page number
    pOemPDEV->PageNum++;

    if(!pOemPDEV->bAppendPageNum)
    {
	    if(IDCANCEL == DialogBoxParam((HINSTANCE)pdevobj->hOEM, MAKEINTRESOURCE(IDD_OUTPUT), NULL,
                                      (DLGPROC)DlgOutputProc, (LPARAM)pdevobj))
            return FALSE;
    }


	// set resolution
	if(pPubDev->dmOrientation == DMORIENT_PORTRAIT)
	{
		pOemPDEV->DIBInfo.XPelsPerMeter = pPubDev->dmPrintQuality * 10000 / 254;
		pOemPDEV->DIBInfo.YPelsPerMeter = pPubDev->dmYResolution * 10000 / 254;
	}
	else
	{
		pOemPDEV->DIBInfo.XPelsPerMeter = pPubDev->dmYResolution * 10000 / 254;
		pOemPDEV->DIBInfo.YPelsPerMeter = pPubDev->dmPrintQuality * 10000 / 254;
	}

	// calculate approximate page height
	if(pPubDev->dmOrientation == DMORIENT_PORTRAIT)
		pOemPDEV->approHeight = pPubDev->dmPaperLength * pPubDev->dmYResolution / 254 + 10;
	else
		pOemPDEV->approHeight = pPubDev->dmPaperWidth * pPubDev->dmPrintQuality / 254 + 10;

	// allocate memory for scanline pointer array
	if(!(pOemPDEV->DIBInfo.pScans = (PSCANLINEPTR)HeapAlloc(pOemPDEV->hHeap, HEAP_ZERO_MEMORY,
															pOemPDEV->approHeight * sizeof(SCANLINEPTR))))
	{
		WARNING(UNITEXT("    error(%d) occured while allocating memory for pScans!\r\n"), GetLastError());
		return FALSE;
	}


    // turn around to call Unidrv
    return (((PFN_DrvStartPage)(pOemPDEV->pfnUnidrv[UD_DrvStartPage]))(pso));
}

/*
 * Function: OEMStartBanding
 *
 * Purpose: Called by GDI when it is ready to start sending bands of a
 *		    physical page to the driver for rendering.
 *
 * Return Value: BOOL
 */
BOOL APIENTRY
OEMStartBanding(
    SURFOBJ *pso,
    POINTL *pptl)
{
    TERSE(UNITEXT("OEMStartBanding() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;

	
    // turn around to call Unidrv
    return (((PFN_DrvStartBanding)(pOemPDEV->pfnUnidrv[UD_DrvStartBanding])) (
			pso,
			pptl
			));
}

/*
 * Function: OEMNextBand
 *
 * Purpose: Called by GDI when it has finished drawing a band for a physical
 *		    page, so the driver can send the band to the printer.
 *
 * Action: Call back to Unidrv, if we are at the end of the page, send
 *		   page to file and clean page-related environment.
 *
 * Return Value: BOOL
 */
BOOL APIENTRY
OEMNextBand(
    SURFOBJ *pso,
    POINTL *pptl)
{
    TERSE(UNITEXT("OEMNextBand() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    BOOL bRet = (((PFN_DrvNextBand)(pOemPDEV->pfnUnidrv[UD_DrvNextBand])) (
				pso,
				pptl
				));

	// see if we are at the end of the page
	if(bRet && (pptl->x == -1L && pptl->y == -1L))
		page_output(pdevobj);

	return bRet;
}

/*
 * Function: OEMSendPage
 *
 * Purpose: Called by GDI when it has finished drawing a physical page, so
 *		    the driver can send the page to the printer.
 *
 * Return Value: BOOL
 */
BOOL APIENTRY
OEMSendPage(
    SURFOBJ    *pso)
{
	TERSE(UNITEXT("OEMSendPage() entry.\r\n"));


	PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    BOOL bRet = (((PFN_DrvSendPage)(pOemPDEV->pfnUnidrv[UD_DrvSendPage]))(pso));

    if(bRet)
	    page_output(pdevobj);

	return bRet;
}

/*
 * Function: OEMEndDoc
 *
 * Purpose: Called by GDI when it has finished sending a document to the
 *		    driver for rendering.
 *
 * Return Value: BOOL
 */
BOOL APIENTRY
OEMEndDoc(
    SURFOBJ    *pso,
    FLONG       fl)
{
    TERSE(UNITEXT("OEMEndDoc() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvEndDoc)(pOemPDEV->pfnUnidrv[UD_DrvEndDoc])) (
			pso,
			fl
			));
}
