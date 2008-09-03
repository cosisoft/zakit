/*
 * @file lib/devmode.cpp Devmode operating function.
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
#include "oem.h"
#include "devmode.h"


/*
 *  Internal implementations
 */

/*
 * Function: fill_default_oemdev
 *
 * Purpose: Fill default oem DEVMODE.
 *
 * Return: size of oem DEVMODE
 */
static DWORD fill_default_oemdev(PDEVMODE pPubDevIn, POEMDEV pOemDevOut)
{
    // verify parameters
    if(pPubDevIn == NULL || pOemDevOut == NULL)
    {
        ERR(ERRORTEXT("fill_default_oemdev(), ERROR_INVALID_PARAMETER.\r\n"));
        return 0;
    }


	// init header
	pOemDevOut->dmKTExtra.dwSize = sizeof(OEMDEV);
	pOemDevOut->dmKTExtra.dwSignature = OEM_SIGNATURE;
	pOemDevOut->dmKTExtra.dwVersion = OEM_VERSION;

    // init peninfo
	for(short Loop = 0; Loop < NUM_COLORS; Loop++)
	{
		pOemDevOut->dmPenInfo[Loop].pibBitmaped = 
		pOemDevOut->dmPenInfo[Loop].pibPath = 
		pOemDevOut->dmPenInfo[Loop].pibText = TRUE;
	}

    // for internal use
    pOemDevOut->dmOldColor = pPubDevIn->dmColor;

	return sizeof(OEMDEV);
}

/*
 * Function: convert_devmode
 *
 * Purpose: Convert oem DEVMODE to current version, if necessary.
 *
 * Return: size of data copied
 */
static DWORD convert_devmode(PDEVMODE pPubDevIn, POEMDEV pOemDevIn, POEMDEV pOemDevOut)
{
    // verify parameters
    if(pPubDevIn == NULL || pOemDevIn == NULL || pOemDevOut == NULL)
    {
        ERR(ERRORTEXT("convert_devmode(), ERROR_INVALID_PARAMETER.\r\n"));
		return 0;
	}


	// check signature, if it doesn't match ours, just assume DMIn is bad and use defaults
	if(pOemDevIn->dmKTExtra.dwSignature == pOemDevOut->dmKTExtra.dwSignature)
	{
		// Step1:
		// Set the devmode defaults so that anything the isn't copied over will
		// be set to the default value.
		fill_default_oemdev(pPubDevIn, pOemDevOut);

		// Step2:
		// Copy the old structure in to the new using which ever size is the smaller.
		// Devmode maybe from newer Devmode (not likely since there is only one), or
		// Devmode maybe a newer Devmode, in which case it maybe larger,
		// but the first part of the structure should be the same.

		// DESIGN ASSUMPTION: the private DEVMODE structure only gets added to;
		// the fields that are in the DEVMODE never change only new fields get added to the end.

		CopyMemory(pOemDevOut, pOemDevIn, __min(pOemDevOut->dmKTExtra.dwSize, pOemDevIn->dmKTExtra.dwSize));

		// Step3:
		// Re-fill in the size and version fields to indicated 
		// that the DEVMODE is the current private DEVMODE version.
		pOemDevOut->dmKTExtra.dwSize       = sizeof(OEMDEV);
		pOemDevOut->dmKTExtra.dwVersion    = OEM_VERSION;

		return __min(pOemDevOut->dmKTExtra.dwSize, pOemDevIn->dmKTExtra.dwSize);
	}
	else
	{
		WARNING(("Unknown DEVMODE signature, pOemDevIn ignored.\r\n"));

		return fill_default_oemdev(pPubDevIn, pOemDevOut);
	}
}

/*
 * Function: validate_devmode
 *
 * Action: Validate oem DEVMODE.
 *
 * Return Value: BOOL
 */
static BOOL validate_devmode(POEMDEV pOemDevOut)
{
	// verify parameters
	if(pOemDevOut == NULL)
		return FALSE;

	// ASSUMPTION: pOemDev is large enough to contain OEMDEV structure

	// make sure that dmKTExtra indicates the current OEMDEV structure
	pOemDevOut->dmKTExtra.dwSize       = sizeof(OEMDEV);
	pOemDevOut->dmKTExtra.dwSignature  = OEM_SIGNATURE;
	pOemDevOut->dmKTExtra.dwVersion    = OEM_VERSION;

	// set OEM DEVMODE to default, if invalid

	return TRUE;
}


/*
 *  Implementations
 */

/*
 * Function: hrOEMDevMode
 *
 * Purpose: Surrogate IOEMUI::DevMode.
 *
 * Return: HRESULT
 */
HRESULT hrOEMDevMode(DWORD dwMode, POEMDMPARAM pOemDMParam)
{
	// verify parameters
	if((NULL == pOemDMParam)
	   ||
	   ((OEMDM_SIZE != dwMode) && (OEMDM_DEFAULT != dwMode) && (OEMDM_CONVERT != dwMode) && (OEMDM_MERGE != dwMode))
	  )
	{
		ERR(ERRORTEXT("DevMode(), ERROR_INVALID_PARAMETER.\r\n"));

		SetLastError(ERROR_INVALID_PARAMETER);
		return E_FAIL;
	}

	// cast generic (i.e. PVOID) to oem devomode pointer type
	POEMDEV pOemDevIn = (POEMDEV)pOemDMParam->pOEMDMIn;
	POEMDEV pOemDevOut = (POEMDEV)pOemDMParam->pOEMDMOut;

	switch(dwMode)
	{
	// return size of the memory needed to store oem DEVMODE
	case OEMDM_SIZE:
		pOemDMParam->cbBufSize = sizeof(OEMDEV);
		break;

	// fill the oem DEVMODE with default value
	case OEMDM_DEFAULT:
		pOemDMParam->cbBufSize = fill_default_oemdev(pOemDMParam->pPublicDMIn, pOemDevOut);
		break;

	case OEMDM_CONVERT:
		pOemDMParam->cbBufSize = convert_devmode(pOemDMParam->pPublicDMIn, pOemDevIn, pOemDevOut);
		break;

	case OEMDM_MERGE:
		convert_devmode(pOemDMParam->pPublicDMIn, pOemDevIn, pOemDevOut);
		validate_devmode(pOemDevOut);
		break;
	}

	return S_OK;
}
