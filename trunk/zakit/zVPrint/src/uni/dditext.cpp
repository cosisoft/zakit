/*
 * @file uni/dditext.cpp DDI hook text function.
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
#include "dditext.h"


/*
 *  Implementations
 */

/*
 * Function: OEMTextOut
 *
 * Purpose: DrvTextOut is the entry point from GDI that calls for the
 *		    driver to render a set of glyphs at specified positions.
 *
 * Return: BOOL
 */
BOOL APIENTRY
OEMTextOut(
    SURFOBJ    *pso,
    STROBJ     *pstro,
    FONTOBJ    *pfo,
    CLIPOBJ    *pco,
    RECTL      *prclExtra,
    RECTL      *prclOpaque,
    BRUSHOBJ   *pboFore,
    BRUSHOBJ   *pboOpaque,
    POINTL     *pptlOrg,
    MIX         mix)
{
	TERSE(UNITEXT("OEMTextOut() entry.\r\n"));


	PDEVOBJ  pdevobj = (PDEVOBJ)pso->dhpdev;
	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;
	POEMDEV  pOemDev = (POEMDEV)pdevobj->pOEMDM;
    short    Index = PCOLOR_BLACK;


    // judge color
    if(pboFore)
        Index = differ_color(pboFore->iSolidColor);
    
	if(pOemDev->dmPenInfo[Index].pibText)
	{
	    // turn around to call Unidrv
	    return (((PFN_DrvTextOut)(pOemPDEV->pfnUnidrv[UD_DrvTextOut])) (
		        pso,
			    pstro,
                pfo,
	            pco,
		        prclExtra,
			    prclOpaque,
                pboFore,
	            pboOpaque,
   			    pptlOrg,
    			mix
	    		));
	}

	return TRUE;
}
