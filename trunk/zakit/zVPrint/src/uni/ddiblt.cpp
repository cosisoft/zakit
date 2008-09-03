/*
 * @file uni/ddiblt.cpp DDI hook block transfer function.
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
#include "ddiblt.h"


/*
 *  Implementations
 */

/*
 * Function: OEMAlphaBlend
 *
 * Purpose: DrvAlphaBlend provides bit-block transfer capabilities with
 *		    alpha blending.
 *
 * Return: BOOL
 *
BOOL APIENTRY
OEMAlphaBlend(
    SURFOBJ    *psoDest,
    SURFOBJ    *psoSrc,
    CLIPOBJ    *pco,
    XLATEOBJ   *pxlo,
    RECTL      *prclDest,
    RECTL      *prclSrc,
    BLENDOBJ   *pBlendObj)
{
	TERSE(UNITEXT("OEMAlphaBlend() entry.\r\n"));


	PDEVOBJ  pdevobj = (PDEVOBJ)psoDest->dhpdev;
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvAlphaBlend)(pOemPDEV->pfnUnidrv[UD_DrvAlphaBlend])) (
            psoDest,
            psoSrc,
            pco,
            pxlo,
            prclDest,
            prclSrc,
            pBlendObj
            ));
}
*/

/*
 * Function: OEMBitBlt
 *
 * Purpose: DrvBitBlt provides general bit-block transfer capabilities
 *		    between device-managed surfaces, between GDI-managed
 *		    standard-format bitmaps, or between a device-managed surface
 *		    and a GDI-managed standard-format bitmap.
 *
 * Return: BOOL
 */
BOOL APIENTRY
OEMBitBlt(
    SURFOBJ        *psoTrg,
    SURFOBJ        *psoSrc,
    SURFOBJ        *psoMask,
    CLIPOBJ        *pco,
    XLATEOBJ       *pxlo,
    RECTL          *prclTrg,
    POINTL         *pptlSrc,
    POINTL         *pptlMask,
    BRUSHOBJ       *pbo,
    POINTL         *pptlBrush,
    ROP4            rop4)
{
	TERSE(UNITEXT("OEMBitBlt() entry.\r\n"));


	PDEVOBJ  pdevobj = (PDEVOBJ)psoTrg->dhpdev;
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;
	POEMDEV	 pOemDev = (POEMDEV)pdevobj->pOEMDM;
    short    Index = PCOLOR_BLACK;


    // judge color
    if(pbo)
        Index = differ_color(pbo->iSolidColor);

	if(pOemDev->dmPenInfo[Index].pibBitmaped)
	{
	    // turn around to call Unidrv
		return (((PFN_DrvBitBlt)(pOemPDEV->pfnUnidrv[UD_DrvBitBlt])) (
			   psoTrg,
			   psoSrc,
		       psoMask,
			   pco,
		       pxlo,
			   prclTrg,
			   pptlSrc,
	           pptlMask,
		       pbo,
			   pptlBrush,
	           rop4
			   ));
	}

	return TRUE;
}

/*
 * Function: OEMCopyBits
 *
 * Purpose: DrvCopyBits translates between device-managed raster surfaces
 *		    and GDI standard-format bitmaps.
 *			GDI will call DrvCopyBits when it needs to copy from one
 *		    surface to another and at least one of the surfaces is
 *		    device-managed
 *
 * Return: BOOL
 *
BOOL APIENTRY
OEMCopyBits(
    SURFOBJ        *psoDest,
    SURFOBJ        *psoSrc,
    CLIPOBJ        *pco,
    XLATEOBJ       *pxlo,
    RECTL          *prclDest,
    POINTL         *pptlSrc)
{
	TERSE(UNITEXT("OEMCopyBits() entry.\r\n"));


	PDEVOBJ  pdevobj = (PDEVOBJ)psoDest->dhpdev;
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvCopyBits)(pOemPDEV->pfnUnidrv[UD_DrvCopyBits])) (
            psoDest,
            psoSrc,
            pco,
            pxlo,
            prclDest,
            pptlSrc
			));
}
*/

/*
 * Function: OEMPlgBlt
 *
 * Purpose: DrvPlgBlt provides rotate bit-block transfer capabilities
 *		    between combinations of device-managed and GDI-managed surfaces.
 *
 * Return: BOOL
 *
BOOL APIENTRY
OEMPlgBlt(
    SURFOBJ         *psoDst,
    SURFOBJ         *psoSrc,
    SURFOBJ         *psoMask,
    CLIPOBJ         *pco,
    XLATEOBJ        *pxlo,
    COLORADJUSTMENT *pca,
    POINTL          *pptlBrushOrg,
    POINTFIX        *pptfixDest,
    RECTL           *prclSrc,
    POINTL          *pptlMask,
    ULONG           iMode)
{
	TERSE(UNITEXT("OEMPlgBlt() entry.\r\n"));


	PDEVOBJ  pdevobj = (PDEVOBJ)psoDst->dhpdev;
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
    return (((PFN_DrvPlgBlt)(pOemPDEV->pfnUnidrv[UD_DrvPlgBlt])) (
            psoDst,
            psoSrc,
            psoMask,
            pco,
            pxlo,
            pca,
            pptlBrushOrg,
            pptfixDest,
            prclSrc,
            pptlMask,
            iMode
			));
}
*/

/*
 * Function: OEMStretchBlt
 *
 * Purpose: DrvStretchBlt provides stretching bit-block transfer capabilities
 *		    between any combination of device-managed and GDI-managed surfaces.
 *
 * Return: BOOL
 */
BOOL APIENTRY
OEMStretchBlt(
    SURFOBJ         *psoDest,
    SURFOBJ         *psoSrc,
    SURFOBJ         *psoMask,
    CLIPOBJ         *pco,
    XLATEOBJ        *pxlo,
    COLORADJUSTMENT *pca,
    POINTL          *pptlHTOrg,
    RECTL           *prclDest,
    RECTL           *prclSrc,
    POINTL          *pptlMask,
    ULONG            iMode)
{
	TERSE(UNITEXT("OEMStretchBlt() entry.\r\n"));


	PDEVOBJ  pdevobj = (PDEVOBJ)psoDest->dhpdev;
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;
	POEMDEV	 pOemDev = (POEMDEV)pdevobj->pOEMDM;


	if(pOemDev->dmPenInfo[PCOLOR_BLACK].pibBitmaped)
	{
	    // turn around to call Unidrv
		return (((PFN_DrvStretchBlt)(pOemPDEV->pfnUnidrv[UD_DrvStretchBlt])) (
		        psoDest,
		        psoSrc,
				psoMask,
				pco,
			    pxlo,
				pca,
			    pptlHTOrg,
	            prclDest,
		        prclSrc,
			    pptlMask,
				iMode
				));
	}

	return TRUE;
}

/*
 * Function: OEMStretchBltROP
 *
 * Purpose: DrvStretchBltROP performs a stretching bit-block transfer
 *		    using a ROP.
 *
 * Return: BOOL
 *
BOOL APIENTRY
OEMStretchBltROP(
    SURFOBJ         *psoDest,
    SURFOBJ         *psoSrc,
    SURFOBJ         *psoMask,
    CLIPOBJ         *pco,
    XLATEOBJ        *pxlo,
    COLORADJUSTMENT *pca,
    POINTL          *pptlHTOrg,
    RECTL           *prclDest,
    RECTL           *prclSrc,
    POINTL          *pptlMask,
    ULONG            iMode,
    BRUSHOBJ        *pbo,
    ROP4             rop4)
{
	TERSE(UNITEXT("OEMStretchBltROP() entry.\r\n"));


	PDEVOBJ  pdevobj = (PDEVOBJ)psoDest->dhpdev;
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
	return (((PFN_DrvStretchBltROP)(pOemPDEV->pfnUnidrv[UD_DrvStretchBltROP])) (
		    psoDest,
			psoSrc,
			psoMask,
	        pco,
		    pxlo,
			pca,
		    pptlHTOrg,
			prclDest,
		    prclSrc,
			pptlMask,
	        iMode,
		    pbo,
			rop4
			));
}
*/

/*
 * Function: OEMTransparentBlt
 *
 * Purpose: DrvTransparentBlt provides bit-block transfer capabilities
 *		    with transparency.
 *
 * Return: BOOL
 *
BOOL APIENTRY
OEMTransparentBlt(
	SURFOBJ    *psoDst,
	SURFOBJ    *psoSrc,
	CLIPOBJ    *pco,
	XLATEOBJ   *pxlo,
	RECTL      *prclDst,
	RECTL      *prclSrc,
	ULONG      iTransColor,
	ULONG      ulReserved)
{
    TERSE(UNITEXT("OEMTransparentBlt() entry.\r\n"));


    PDEVOBJ  pdevobj = (PDEVOBJ)psoDst->dhpdev;
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


    // turn around to call Unidrv
	return (((PFN_DrvTransparentBlt)(pOemPDEV->pfnUnidrv[UD_DrvTransparentBlt])) (
		    psoDst,
			psoSrc,
            pco,
	        pxlo,
		    prclDst,
			prclSrc,
            iTransColor,
	        ulReserved
		    ));
}
*/
