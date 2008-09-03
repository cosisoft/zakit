/*
 * @file uni/ddimisc.cpp DDI hook misc function.
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
#include "ddimisc.h"


/*
 *  Implementations
 */

/*
 * Function: OEMDitherColor
 *
 * Purpose: DrvDitherColor requests the device to create a brush dithered
 *		    against a device palette.
 *
 * Return: ULONG
 *
ULONG APIENTRY
OEMDitherColor(
    DHPDEV  dhpdev,
    ULONG   iMode,
    ULONG   rgbColor,
    ULONG  *pulDither)
{
    TERSE(UNITEXT("OEMDitherColor() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvDitherColor)(pOemPDEV->pfnUnidrv[UD_DrvDitherColor])) (
            dhpdev,
            iMode,
            rgbColor,
            pulDither
			));
}
*/

/*
 * Function: OEMEscape
 *
 * Purpose: DrvEscape is used for retrieving information from a device that
 *		    is not available in a device-independent device driver interface.
 *		    The particular query depends on the value of the iEsc parameter.
 *		    Drawing on the device is not allowed in this function.
 *		    DrvDrawEscape is to be used for specialized drawing support.
 *
 * Return: BOOL
 *
ULONG APIENTRY
OEMEscape(
    SURFOBJ    *pso,
    ULONG       iEsc,
    ULONG       cjIn,
    PVOID       pvIn,
    ULONG       cjOut,
    PVOID       pvOut)
{
	TERSE(UNITEXT("OEMEscape() entry.\r\n"));


	PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvEscape)(pOemPDEV->pfnUnidrv[UD_DrvEscape])) (
            pso,
            iEsc,
            cjIn,
            pvIn,
            cjOut,
            pvOut
			));
}
*/

/*
 * Function: OEMPaint
 *
 * Purpose: 
 *
 * Return: BOOL
 *
BOOL APIENTRY
OEMPaint(
    SURFOBJ         *pso,
    CLIPOBJ         *pco,
    BRUSHOBJ        *pbo,
    POINTL          *pptlBrushOrg,
    MIX             mix)
{
    TERSE(UNITEXT("OEMPaint() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvPaint)(pOemPDEV->pfnUnidrv[UD_DrvPaint])) (
            pso,
            pco,
            pbo,
            pptlBrushOrg,
            mix
			));
}
*/

/*
 * Function: KTRealizeBrush
 *
 * Purpose: DrvRealizeBrush requests that the driver realize a specified
 *		    brush for a specified surface.
 *
 * Return: BOOL
 *
BOOL APIENTRY
KTRealizeBrush(
    BRUSHOBJ   *pbo,
    SURFOBJ    *psoTarget,
    SURFOBJ    *psoPattern,
    SURFOBJ    *psoMask,
    XLATEOBJ   *pxlo,
    ULONG       iHatch)
{
    TERSE(UNITEXT("KTRealizeBrush() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)psoTarget->dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;

    //
    // the OEM DLL should NOT hook out this function unless it wants to draw
    // graphics directly to the device surface. In that case, it calls
    // EngRealizeBrush which causes GDI to call DrvRealizeBrush.
    // Note that it cannot call back into Unidrv since Unidrv doesn't hook it.
    //

    //
    // In this DLL, the drawing hooks does not call EngRealizeBrush. So this
    // this function will never be called. Do nothing.
    //

    return TRUE;
}
*/
