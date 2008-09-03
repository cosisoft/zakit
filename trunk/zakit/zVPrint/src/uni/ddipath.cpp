/*
 * @file uni/ddipath.cpp DDI hook path function.
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
#include "color.h"
#include "ddipath.h"


/*
 *  Implementations
 */

/*
 * Function: OEMFillPath
 *
 * Purpose: DrvFillPath is an optional entry point(according to flags in
 *          DEVINFO) to handle the filling of closed paths.
 *
 * Return: BOOL
 *
BOOL APIENTRY
OEMFillPath(
    SURFOBJ    *pso,
    PATHOBJ    *ppo,
    CLIPOBJ    *pco,
    BRUSHOBJ   *pbo,
    POINTL     *pptlBrushOrg,
    MIX         mix,
    FLONG       flOptions)
{
    TERSE(UNITEXT("OEMFillPath() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvFillPath)(pOemPDEV->pfnUnidrv[UD_DrvFillPath])) (
            pso,
            ppo,
            pco,
            pbo,
            pptlBrushOrg,
            mix,
            flOptions
			));
}
*/

/*
 * Function: OEMGradientFill
 *
 * Purpose: DrvGradientFill shades the specified primitives.
 *
 * Return: BOOL
 *
BOOL APIENTRY
OEMGradientFill(
	SURFOBJ    *psoDest,
	CLIPOBJ    *pco,
	XLATEOBJ   *pxlo,
	TRIVERTEX  *pVertex,
	ULONG       nVertex,
	PVOID       pMesh,
	ULONG       nMesh,
	RECTL      *prclExtents,
	POINTL     *pptlDitherOrg,
	ULONG       ulMode)
{
    TERSE(UNITEXT("OEMGradientFill() entry.\r\n"));


	PDEVOBJ  pdevobj = (PDEVOBJ)psoDest->dhpdev;
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvGradientFill)(pOemPDEV->pfnUnidrv[UD_DrvGradientFill])) (
            psoDest,
            pco,
            pxlo,
            pVertex,
            nVertex,
            pMesh,
            nMesh,
            prclExtents,
            pptlDitherOrg,
            ulMode
            ));
}
*/

/*
 * Function: OEMLineTo
 *
 * Purpose: DrvLineTo draws a single, solid, integer-only cosmetic line.
 *
 * Return: BOOL
 *
BOOL APIENTRY
OEMLineTo(
    SURFOBJ    *pso,
    CLIPOBJ    *pco,
    BRUSHOBJ   *pbo,
    LONG        x1,
    LONG        y1,
    LONG        x2,
    LONG        y2,
    RECTL      *prclBounds,
    MIX         mix)
{
    TERSE(UNITEXT("OEMLineTo() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
	return (((PFN_DrvLineTo)(pOemPDEV->pfnUnidrv[UD_DrvLineTo])) (
            pso,
            pco,
            pbo,
            x1,
            y1,
            x2,
            y2,
            prclBounds,
            mix
			));
}
*/

/*
 * Function: OEMStrokeAndFillPath
 *
 * Purpose: DrvStrokeAndFillPath strokes and fills a path concurrently
 *          (according to flags in DEVINFO structure).
 *
 * Return: BOOL
 *
BOOL APIENTRY
OEMStrokeAndFillPath(
    SURFOBJ    *pso,
    PATHOBJ    *ppo,
    CLIPOBJ    *pco,
    XFORMOBJ   *pxo,
    BRUSHOBJ   *pboStroke,
    LINEATTRS  *plineattrs,
    BRUSHOBJ   *pboFill,
    POINTL     *pptlBrushOrg,
    MIX         mixFill,
    FLONG       flOptions)
{
	TERSE(UNITEXT("OEMStrokeAndFillPath() entry.\r\n"));


	PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvStrokeAndFillPath)(pOemPDEV->pfnUnidrv[UD_DrvStrokeAndFillPath])) (
            pso,
            ppo,
            pco,
            pxo,
            pboStroke,
            plineattrs,
            pboFill,
            pptlBrushOrg,
            mixFill,
            flOptions
			));
}
*/

/*
 * Function: OEMStrokePath
 *
 * Purpose: The DrvStrokePath function strokes (outlines) a path(according to
 *          flags in DEVINFO structure).
 *
 * Return: BOOL
 */
BOOL APIENTRY
OEMStrokePath(
    SURFOBJ    *pso,
    PATHOBJ    *ppo,
    CLIPOBJ    *pco,
    XFORMOBJ   *pxo,
    BRUSHOBJ   *pbo,
    POINTL     *pptlBrushOrg,
    LINEATTRS  *plineattrs,
    MIX         mix)
{
    TERSE(UNITEXT("OEMStokePath() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;
	POEMDEV	 pOemDev = (POEMDEV)pdevobj->pOEMDM;
	short	 Index = PCOLOR_BLACK;


	// judge color
	if(pbo)
		Index = differ_color(pbo->iSolidColor);

	if(pOemDev->dmPenInfo[Index].pibPath)
	{
	    // turn around to call Unidrv
		return (((PFN_DrvStrokePath)(pOemPDEV->pfnUnidrv[UD_DrvStrokePath])) (
			    pso,
				ppo,
				pco,
		        pxo,
			    pbo,
				pptlBrushOrg,
			    plineattrs,
				mix
				));
	}

	return TRUE;
}
