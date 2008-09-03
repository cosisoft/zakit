/*
 * @file uni/ddifont.cpp DDI hook font management funcion.
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
#include "ddifont.h"


/*
 *  Implementations
 */

////////
// NOTE:
// OEM DLL needs to hook out the following six font related DDI calls only
// if it enumerates additional fonts beyond what's in the GPD file.
// And if it does, it needs to take care of its own fonts for all font DDI
// calls and DrvTextOut call.
///////

/*
 * Function: OEMFontManagement
 *
 * Purpose: DrvFontManagement is an optional entry point provided for
 *		    PostScript devices. If this function is hooked by the device
 *		    driver then GDI will pass calls made by an application to
 *		    ExtEscape for escape numbers 0x100 through 0x3fe, or for the
 *		    QUERYESCSUPPORT escape when the first DWORD pointed to by pvIn
 *		    is in the range 0x100 through 0x3fe.
 *
 * Return: ULONG
 *
ULONG APIENTRY
OEMFontManagement(
    SURFOBJ    *pso,
    FONTOBJ    *pfo,
    ULONG       iMode,
    ULONG       cjIn,
    PVOID       pvIn,
    ULONG       cjOut,
    PVOID       pvOut)
{
    TERSE(UNITEXT("OEMFontManagement() entry.\r\n"));

    //
    // Note that Unidrv will not call OEM DLL for iMode==QUERYESCSUPPORT.
    // So pso is not NULL for sure.
    //
    PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv if this is not the font that OEM enumerated.
    return (((PFN_DrvFontManagement)(pOemPDEV->pfnUnidrv[UD_DrvFontManagement])) (
            pso,
            pfo,
            iMode,
            cjIn,
            pvIn,
            cjOut,
            pvOut
			));
}
*/

/*
 * Function: OEMGetGlyphMode
 *
 * Purpose: DrvGetGlyphMode tells GDI how to cache glyph information.
 *
 * Return: ULONG
 *
ULONG APIENTRY
OEMGetGlyphMode(
    DHPDEV      dhpdev,
    FONTOBJ    *pfo)
{
    TERSE(UNITEXT("OEMGetGlyphMode() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv if this is not the font that OEM enumerated.
    return (((PFN_DrvGetGlyphMode)(pOemPDEV->pfnUnidrv[UD_DrvGetGlyphMode])) (
            dhpdev,
            pfo
			));
}
*/

/*
 * Function: OEMQueryAdvanceWidths
 *
 * Purpose: DrvQueryAdvanceWidths returns character advance widths for a
 *		    specified set of glyphs.
 *
 * Return: BOOL
 *
BOOL APIENTRY
OEMQueryAdvanceWidths(
    DHPDEV      dhpdev,
    FONTOBJ    *pfo,
    ULONG       iMode,
    HGLYPH     *phg,
    PVOID       pvWidths,
    ULONG       cGlyphs)
{
    TERSE(UNITEXT("OEMQueryAdvanceWidths() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv if this is not the font that OEM enumerated.
    return (((PFN_DrvQueryAdvanceWidths)(pOemPDEV->pfnUnidrv[UD_DrvQueryAdvanceWidths])) (
            dhpdev,
            pfo,
            iMode,
            phg,
            pvWidths,
            cGlyphs
		    ));
}
*/

/*
 * Function: OEMQueryFont
 *
 * Purpose: DrvQueryFont is used by GDI to get the IFIMETRICS structure for
 *		    a given font. The driver fills the IFIMETRICS structure.
 *
 * Return: PIFIMETRICS
 *
PIFIMETRICS APIENTRY
OEMQueryFont(
    DHPDEV      dhpdev,
    ULONG       iFile,
    ULONG       iFace,
    ULONG      *pid)
{
    TERSE(UNITEXT("OEMQueryFont() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvQueryFont)(pOemPDEV->pfnUnidrv[UD_DrvQueryFont])) (
            dhpdev,
            iFile,
            iFace,
            pid
			));
}
*/

/*
 * Function: OEMQueryFontData
 *
 * Action: DrvQueryFontData retrieves information about a realized font.
 *		   GDI provides a pointer to an array of glyph or kerning handles,
 *		   and the driver returns information about the glyphs or kerning
 *		   pairs. The driver can assume that all handles in the array are
 *		   valid.
 *
 * Return: PVOID
 *
LONG APIENTRY
OEMQueryFontData(
    DHPDEV      dhpdev,
    FONTOBJ    *pfo,
    ULONG       iMode,
    HGLYPH      hg,
    GLYPHDATA  *pgd,
    PVOID       pv,
    ULONG       cjSize)
{
    TERSE(UNITEXT("OEMQueryFontData() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv if this is not the font that OEM enumerated.
    return (((PFN_DrvQueryFontData)(pOemPDEV->pfnUnidrv[UD_DrvQueryFontData])) (
            dhpdev,
            pfo,
            iMode,
            hg,
            pgd,
            pv,
            cjSize
			));
}
*/

/*
 * Function: OEMQueryFontTree
 *
 * Action: When GDI calls DrvQueryFontTree, the driver supplies pointers
 *		   to structures that define one of the following: 
 *		   Mapping from Unicode to glyph handles, including glyph variants 
 *		   Mapping of kerning pairs to kerning handles
 *
 * Return: PVOID
 *
PVOID APIENTRY
OEMQueryFontTree(
    DHPDEV      dhpdev,
    ULONG       iFile,
    ULONG       iFace,
    ULONG       iMode,
    ULONG      *pid)
{
    TERSE(UNITEXT("OEMQueryFontTree() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvQueryFontTree)(pOemPDEV->pfnUnidrv[UD_DrvQueryFontTree])) (
            dhpdev,
            iFile,
            iFace,
            iMode,
            pid
			));
}
*/
