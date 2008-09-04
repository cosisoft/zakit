/*
 * @file uni/output.cpp Output function.
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
#include "output.h"


/*
 *  Internal prototypes
 */

static void append_page_num(PTSTR pPreviewFileName, PTSTR pNameBuffer, short PageNum);
static void write_dib_header(PDEVOBJ pdevobj, HANDLE hFile, BOOL bWrite);


/*
 *  Internal implementations
 */

/*
 * Function: append_page_num
 *
 * Purpose: Append page number on file name automatically.
 *
 * Return: NULL
 */
static void append_page_num(PTSTR pPreviewFileName, PTSTR pNameBuffer, short PageNum)
{
	lstrcpy(pNameBuffer, pPreviewFileName);

	int FileNameLen = lstrlen(pNameBuffer),
	    Loop = 0;

	for(Loop = FileNameLen - 1; Loop >=0 ; Loop--)
		if(pNameBuffer[Loop] == '.')
			break;

	wsprintf((pNameBuffer + Loop), TEXT("_p%d.bmp\0"), PageNum);
}

/*
 * Function: write_dib_header
 *
 * Purpose: Write DIB header to file.
 *
 * Return Value: NULL
 */
static void write_dib_header(PDEVOBJ pdevobj, HANDLE hFile, BOOL bWrite)
{
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;
    PDEVMODE pPubDev = (PDEVMODE)pdevobj->pPublicDM;
	DWORD    Offset = 0,
		     DIBHdrSize = 0;


    if(pPubDev->dmColor == DMCOLOR_COLOR) // 24bpp
        DIBHdrSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    else // 1bpp
        DIBHdrSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2 * sizeof(RGBQUAD);

	if(!bWrite) // offset file pointer
	{
		Offset = SetFilePointer(hFile, DIBHdrSize, NULL, FILE_BEGIN);
	}
	else // write DIB file header
	{
		BITMAPFILEHEADER bmfh = {0};

		// init BITMAPFILEHEADER
		bmfh.bfType = 0x4D42;
		bmfh.bfSize = DIBHdrSize + pOemPDEV->DIBInfo.dwDIBTotalBytes;
		bmfh.bfReserved1 =
		bmfh.bfReserved2 = 0;
		bmfh.bfOffBits = DIBHdrSize;


		BITMAPINFOHEADER bmi = {0};

		// init BITMAPINFOHEADER
		bmi.biSize          = sizeof(BITMAPINFOHEADER);
		bmi.biWidth         = pOemPDEV->DIBInfo.DIBSize.cx;
		bmi.biHeight        = pOemPDEV->DIBInfo.DIBSize.cy;
		bmi.biPlanes        = 1;
        if(pPubDev->dmColor == DMCOLOR_COLOR) // 24bpp
		    bmi.biBitCount  = 24;
        else // 1bpp
            bmi.biBitCount  = 1;
		bmi.biCompression   = BI_RGB;
		bmi.biSizeImage     = 0;
		bmi.biXPelsPerMeter = pOemPDEV->DIBInfo.XPelsPerMeter;
		bmi.biYPelsPerMeter = pOemPDEV->DIBInfo.YPelsPerMeter;
		bmi.biClrUsed       = 0;
		bmi.biClrImportant  = 0;


    	BOOL bRet = FALSE;
		DWORD NumByteWritten = 0;

  		// offset file pointer
		Offset = SetFilePointer(hFile, 0 , NULL, FILE_BEGIN);

		// write BITMAPFILEHEADER to DIB file
		bRet = WriteFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &NumByteWritten, NULL);

		// write BITMAPINFOHEADER to DIB file
		bRet = WriteFile(hFile, &bmi, sizeof(BITMAPINFOHEADER), &NumByteWritten, NULL);


        if(pPubDev->dmColor == DMCOLOR_MONOCHROME) // 1bpp
        {
		    RGBQUAD color = {0};

		    // black, index 0
		    color.rgbRed = 0;
		    color.rgbGreen = 0;
		    color.rgbBlue = 0;
		    bRet = WriteFile(hFile, &color, sizeof(RGBQUAD), &NumByteWritten, NULL);

		    // white, index 1
		    color.rgbRed = 255;
		    color.rgbGreen = 255;
		    color.rgbBlue = 255;
		    bRet = WriteFile(hFile, &color, sizeof(RGBQUAD), &NumByteWritten, NULL);
        }
	}
}


/*
 *  Implementations
 */

/*
 * Function: convert_to_normal_dib
 *
 * Purpose: Convert dump DIB to normal down-top DIB.
 *
 * Return Value: if success, return TRUE; otherwise, return FALSE
 */
BOOL convert_to_normal_dib(PDEVOBJ pdevobj)
{
	TERSE(UNITEXT("convert_to_normal_dib() entry.\r\n"));


	POEMPDEV  pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;
	PDEVMODE  pPubDev = (PDEVMODE)pdevobj->pPublicDM;
    HANDLE    hDIBFile = NULL;
	LONG	  Loop = 0;
    // for error checking
	BOOL	  bRet = FALSE;
	DWORD	  NumBytesWritten = 0;


	// create DIB file
    if(pOemPDEV->bAppendPageNum)
    {
        TCHAR szNameBuffer[MAX_PATH] = {0};

        append_page_num(pOemPDEV->szPreviewFileName, szNameBuffer, pOemPDEV->PageNum);
        hDIBFile = CreateFile(szNameBuffer, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    else
	    hDIBFile = CreateFile(pOemPDEV->szPreviewFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hDIBFile == INVALID_HANDLE_VALUE)
	{
		WARNING(UNITEXT("    error(%d) occured while creating file!\r\n"), GetLastError());
		return FALSE;
	}

	// offset file pointer
	write_dib_header(pdevobj, hDIBFile, FALSE);

	// conver for 24bpp
	if(pPubDev->dmColor == DMCOLOR_COLOR) // 24bpp
	{
		for(Loop = pOemPDEV->DIBInfo.DIBSize.cy-1; Loop > 0;Loop--)
		{
			DWORD index=0;
			BYTE temp;
			while(index < (pOemPDEV->DIBInfo.WidthBytes) )
			{
				temp=pOemPDEV->DIBInfo.pScans[Loop].pBits[index];
				pOemPDEV->DIBInfo.pScans[Loop].pBits[index]=pOemPDEV->DIBInfo.pScans[Loop].pBits[index+2];
				pOemPDEV->DIBInfo.pScans[Loop].pBits[index+2]=temp;
				index+=3;
			}
		}
	}

    // convert
    for(Loop = pOemPDEV->DIBInfo.DIBSize.cy; Loop > 0;)
        bRet = WriteFile(hDIBFile, pOemPDEV->DIBInfo.pScans[--Loop].pBits, pOemPDEV->DIBInfo.WidthBytes, &NumBytesWritten, NULL);

	// write DIB header and close file
	write_dib_header(pdevobj, hDIBFile, TRUE);
	CloseHandle(hDIBFile);

	return TRUE;
}
