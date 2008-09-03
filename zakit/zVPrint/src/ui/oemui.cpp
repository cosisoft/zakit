/*
 * @file ui/oemui.cpp OEM UI function.
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
#include "globals.h"
#include "pageclbk.h"
#include "resource.h"
#include "oemui.h"


/*
 *  Internal implementations
 */

UINT CALLBACK OEMDocumentPropSheetPageProc(HWND hWnd, UINT uMsg, LPPROPSHEETPAGE ppsp);

static void hide_system_option(POEMCUIPPARAM pOemCUIPParam);
static HRESULT modify_doc_prop(POEMCUIPPARAM pOemCUIPParam);


/*
 *  Internal implementations
 */

/*
 * Function: hide_system_option
 *
 * Purpose: Hide some system options.
 *
 * Return: NULL
 */
static void hide_system_option(POEMCUIPPARAM pOemCUIPParam)
{
	for(DWORD Loop = 0; Loop < pOemCUIPParam->cDrvOptItems; Loop++)
	{
		switch(pOemCUIPParam->pDrvOptItems[Loop].DMPubID)
		{
//		case DMPUB_ORIENTATION:
		case DMPUB_PAGEORDER:
		case DMPUB_NUP:
		case DMPUB_DEFSOURCE:
//		case DMPUB_COLOR:
//		case DMPUB_FORMNAME:
		case DMPUB_COPIES_COLLATE:
//		case DMPUB_PRINTQUALITY:
		case DMPUB_ICMINTENT:
		case DMPUB_ICMMETHOD:
			pOemCUIPParam->pDrvOptItems[Loop].Flags |= OPTIF_HIDE;
			break;

		case DMPUB_NONE:
			if(Loop != pOemCUIPParam->cDrvOptItems - 1)
				pOemCUIPParam->pDrvOptItems[Loop].Flags |= OPTIF_HIDE;

			break;
		}
	}
}

/*
 * Function: modify_doc_prop
 *
 * Purpose: Modify doc property pages.
 *
 * Return: HRESULT
 */
static HRESULT modify_doc_prop(POEMCUIPPARAM pOemCUIPParam)
{
	if(pOemCUIPParam->pDrvOptItems != NULL)
		hide_system_option(pOemCUIPParam);

	return S_OK;
}

/*
 * Function: doc_release_psp
 *
 * Purpose: Release resources used by document property page.
 *
 * Return: NULL
 */
static void doc_release_psp(PDIALOGINFO pdi)
{
	if(pdi)
	{
		if(!pdi->bOK) // discard modifications
		{
			// oem devmode
			if(pdi->pOemDevOrig)
				CopyMemory(pdi->pOemDev, pdi->pOemDevOrig, sizeof(OEMDEV));
		}

		if(pdi->pOemDevOrig)
			HeapFree(pdi->hHeap, 0, pdi->pOemDevOrig);

		HeapFree(pdi->hHeap, 0, pdi);
	}
}


/*
 *  Implementations
 */

/*
 * Function: hrOEMCommonUIProp
 *
 * Purpose: Surrogate IKTUI::CommonUIProp.
 *
 * Return: HRESULT
 */
HRESULT hrOEMCommonUIProp(
	DWORD			dwMode,
	POEMCUIPPARAM   pOemCUIPParam)
{
	// validate parameters
	if((dwMode != OEMCUIP_DOCPROP && dwMode != OEMCUIP_PRNPROP) || pOemCUIPParam == NULL)
	{
		ERR(UIERROR("hrOEMCommonUIProp() ERROR_INVALID_PARAMETER.\r\n"));

		SetLastError(ERROR_INVALID_PARAMETER);
		return E_FAIL;
    }


	HRESULT hResult = S_OK;

	switch(dwMode)
	{
	case OEMCUIP_DOCPROP:
		hResult = modify_doc_prop(pOemCUIPParam);
		break;
	}

	return hResult;
}

/*
 * Function: hrOEMDevicePropertySheets
 *
 * Purpose: Surrogate IKTUI::DevicePropertySheets.
 *
 * Return: HRESULT
 */
HRESULT hrOEMDevicePropertySheets(
	PPROPSHEETUI_INFO pPSUIInfo,
	LPARAM lParam)
{
    // validate parameters
	if(pPSUIInfo == NULL || pPSUIInfo->Version != PROPSHEETUI_INFO_VERSION)
	{
		ERR(UIERROR("hrOEMDevicePropertySheets() ERROR_INVALID_PARAMETER.\r\n"));

		SetLastError(ERROR_INVALID_PARAMETER);
		return E_FAIL;
	}


	LONG_PTR    lResult;

	switch(pPSUIInfo->Reason)
	{
	// init, add about page
	case PROPSHEETUI_REASON_INIT:
		{
			PROPSHEETPAGE   Page = {0};

			// init property pages
			Page.dwSize = sizeof(PROPSHEETPAGE);
			Page.dwFlags = PSP_DEFAULT;
			Page.hInstance = ghInstance;
			Page.pszTemplate = MAKEINTRESOURCE(IDD_ABOUT);
			Page.pfnDlgProc = (DLGPROC)DlgAboutProc;

			// add property page
			lResult = pPSUIInfo->pfnComPropSheet(pPSUIInfo->hComPropSheet, CPSFUNC_ADD_PROPSHEETPAGE, (LPARAM)&Page, 0);
		}
		break;

	// no action
	case PROPSHEETUI_REASON_GET_INFO_HEADER:
		lResult = TRUE;
		break;

	// no action
	case PROPSHEETUI_REASON_GET_ICON:
		lResult = TRUE;
		break;

	// receive result information in lParam(SETRESULT_INFO)
	case PROPSHEETUI_REASON_SET_RESULT:
		{
			PSETRESULT_INFO pInfo = (PSETRESULT_INFO) lParam;

			lResult = pInfo->Result;
		}
		break;

	// release resources allocated in PROPSHEETUI_REASON_INIT process
	case PROPSHEETUI_REASON_DESTROY:
		lResult = TRUE;
		break;
	}

	pPSUIInfo->Result = lResult;
	return S_OK;
}

/*
 * Function: hrOEMDocumentPropertySheets
 *
 * Purpose: Surrogate IKTUI::DocumentPropertySheets.
 *
 * Return: HRESULT
 */
HRESULT hrOEMDocumentPropertySheets(
	PPROPSHEETUI_INFO	pPSUIInfo,
	LPARAM				lParam)
{
	// validate parameters
	if(pPSUIInfo == NULL || pPSUIInfo->Version != PROPSHEETUI_INFO_VERSION)
	{
		ERR(UIERROR("hrOEMDocumentPropertySheets() ERROR_INVALID_PARAMETER.\r\n"));

		SetLastError(ERROR_INVALID_PARAMETER);
		return  E_FAIL;
	}


	LONG_PTR    lResult;

	switch(pPSUIInfo->Reason)
	{
	// init, add private property page
	case PROPSHEETUI_REASON_INIT:
		{
			POEMUIPSPARAM   pOEMUIParam = (POEMUIPSPARAM) pPSUIInfo->lParamInit;
			HANDLE          hHeap = pOEMUIParam->hOEMHeap;
			POEMDEV			pOemDev = (POEMDEV)pOEMUIParam->pOEMDM;
			PDIALOGINFO		pdi = NULL;

			// init dialog information
			if(pdi = (PDIALOGINFO)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(DIALOGINFO)))
			{
				pdi->pPubDev = pOEMUIParam->pPublicDM;
				pdi->pOemDev = pOemDev;
				if(pdi->pOemDevOrig = (POEMDEV)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(OEMDEV)))
					CopyMemory(pdi->pOemDevOrig, pOemDev, sizeof(OEMDEV));
				pdi->hHeap = hHeap;
				pdi->bOK = FALSE;
			}


			PROPSHEETPAGE   Page = {0};

			// init property pages
			Page.dwSize = sizeof(PROPSHEETPAGE);
			Page.dwFlags = PSP_USECALLBACK;
			Page.hInstance = ghInstance;
			Page.pszTemplate = MAKEINTRESOURCE(IDD_CUSTOM);
			Page.pfnDlgProc = (DLGPROC)DlgCustomProc;
			Page.lParam = (LPARAM)pdi;
			Page.pfnCallback = (LPFNPSPCALLBACK)OEMDocumentPropSheetPageProc;

			// add property page
			lResult = pPSUIInfo->pfnComPropSheet(pPSUIInfo->hComPropSheet, CPSFUNC_ADD_PROPSHEETPAGE, (LPARAM)&Page, 0);
		}
		break;

	// no action
	case PROPSHEETUI_REASON_GET_INFO_HEADER:
		lResult = TRUE;
		break;

	// no action
	case PROPSHEETUI_REASON_GET_ICON:
		lResult = TRUE;
		break;

	// receive result in lParam(SETRESULT_INFO)
	case PROPSHEETUI_REASON_SET_RESULT:
		{
			PSETRESULT_INFO pInfo = (PSETRESULT_INFO) lParam;

			lResult = pInfo->Result;
		}
		break;

	// release resources allocated in PROPSHEETUI_REASON_INIT process
	case PROPSHEETUI_REASON_DESTROY:
		lResult = TRUE;
		break;
    }

	pPSUIInfo->Result = lResult;
	return S_OK;
}

/*
 * Function: OEMDocumentPropSheetPageProc
 *
 * Purpose: Callback for document property sheet page.
 *
 * Return: always 1
 */
UINT CALLBACK OEMDocumentPropSheetPageProc(HWND hWnd, UINT uMsg, LPPROPSHEETPAGE ppsp)
{
	if(uMsg == PSPCB_RELEASE)
		doc_release_psp((PDIALOGINFO)ppsp->lParam);

	return 1;
}
