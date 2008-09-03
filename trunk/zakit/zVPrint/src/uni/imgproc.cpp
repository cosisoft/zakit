/*
 * @file uni/imgproc.cpp Image process function.
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
#include "imgproc.h"


/*
 *  Implementations
 */

/*
 * Function: dump
 *
 * Purpose: 
 *
 * Return: if success, return TRUE; otherwise, return FALSE
 */
BOOL dump(PDEVOBJ pdevobj, PBYTE pSrcBmp, PBITMAPINFOHEADER pInfoHeader, PIPPARAMS pIPParams)
{
	TERSE(UNITEXT("dump() entry.\r\n"));


	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;
	LONG	 Loop = 0;
	// for error checking
	BOOL	 bRet = FALSE;
	DWORD	 NumBytesWritten = 0;


	for(Loop = 0; Loop < pInfoHeader->biHeight; Loop++)
	{
		if(pOemPDEV->DIBInfo.pScans[pOemPDEV->DIBInfo.DIBSize.cy].pBits
				= (PBYTE)HeapAlloc(pOemPDEV->hHeap, HEAP_ZERO_MEMORY, pOemPDEV->DIBInfo.WidthBytes))
		{
			// set flag
			pOemPDEV->DIBInfo.pScans[pOemPDEV->DIBInfo.DIBSize.cy].bFlag = TRUE;

    	    if(!pIPParams->bBlankBand) // ~blank
	        {
                CopyMemory(pOemPDEV->DIBInfo.pScans[pOemPDEV->DIBInfo.DIBSize.cy].pBits, pSrcBmp, pOemPDEV->DIBInfo.WidthBytes);
                pSrcBmp += pOemPDEV->DIBInfo.WidthBytes; // offset pointer to new scanline
            }
            else
            {
				// fill white
				FillMemory(pOemPDEV->DIBInfo.pScans[pOemPDEV->DIBInfo.DIBSize.cy].pBits, pOemPDEV->DIBInfo.WidthBytes, 0xff);
            }

            pOemPDEV->DIBInfo.dwDIBTotalBytes += pOemPDEV->DIBInfo.WidthBytes; // update DIB total bytes
    		pOemPDEV->DIBInfo.DIBSize.cy++; // update DIB height
	   	}
		else
		{
			WARNING(UNITEXT("    error(%d) occured while allocating memory for pScans[height].pBits!\r\n"), GetLastError());
			return FALSE;
		}
	}

	return TRUE;
}
