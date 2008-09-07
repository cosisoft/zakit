/*
 * @file uni/dlgclbk.cpp Dialog callback function.
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
#include "oemuni.h"
#include "devmode.h"
#include "resource.h"
#include "dlgclbk.h"


/*
 *  Internal datas
 */

static TCHAR szStrResource[MAX_PATH];


/*
 *  Internal prototypes
 */

static PCTSTR get_resource_string(UINT uID);
static void dlg_error_message(HWND hDlg, UINT MessageID, UINT TitleID);

static DWORD output_get_filename(HWND hDlg, PDEVOBJ pdevobj);
static BOOL output_validate_filename(HWND hDlg, PDEVOBJ pdevobj);


/*
 *  Implementations
 */

/*
 * Function: DlgOutputProc
 *
 * Purpose: Call back function for IDD_OUTPUT dialog.
 *
 * Return: BOOL value
 */
BOOL FAR PASCAL DlgOutputProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	PDEVOBJ  pdevobj = NULL;
    POEMPDEV pOemPDEV = NULL;


	switch(message)
	{
	case WM_INITDIALOG:
		pdevobj = (PDEVOBJ)lParam;
		SetWindowLong(hDlg, GWL_USERDATA, (LONG)pdevobj);
		
		// set checked option
		CheckDlgButton(hDlg,IDC_APPEND_PAGENUM,TRUE);
		((POEMPDEV)pdevobj->pdevOEM)->bAppendPageNum = TRUE;

		// center and top-most
		RECT  rcDlg;  
		int   cxDlg,cyDlg;  
		::GetWindowRect(hDlg,&rcDlg);  
		cxDlg=::GetSystemMetrics(SM_CXSCREEN)/2-(rcDlg.right-rcDlg.left)/2;
		cyDlg=::GetSystemMetrics(SM_CYSCREEN)/2-(rcDlg.bottom-rcDlg.top)/2;
		SetForegroundWindow(hDlg);
		::SetWindowPos(hDlg,HWND_TOPMOST,cxDlg,cyDlg,0,0,SWP_NOSIZE);

		break;

	case WM_COMMAND:
		pdevobj = (PDEVOBJ)GetWindowLong(hDlg, GWL_USERDATA);
        pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;

		switch(wParam)
		{
        // check "browse" button
        case IDC_BROWSE:
            {
				if(output_get_filename(hDlg, pdevobj) == SAVE_FAILED)
					dlg_error_message(hDlg, IDS_BROWSE_ERROR, IDS_OUTPUT);
            }
            break;

        // check "append page num" button
        case IDC_APPEND_PAGENUM:
            pOemPDEV->bAppendPageNum ^= 1;
            break;

        // check "start" button
		case IDOK:
            if(output_validate_filename(hDlg, pdevobj)) // validate file name
            {
			    EndDialog(hDlg, IDOK);
            }
            else
            {
                dlg_error_message(hDlg, IDS_FILENAME_INVALID, IDS_OUTPUT);

                SetFocus(GetDlgItem(hDlg, IDC_FILENAME));
                SendDlgItemMessage(hDlg, IDC_FILENAME, EM_SETSEL, 0, -1L);
            }

			break;

        // check "cancel" button
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


/*
 *  Internal implementations
 */

/*
 * Function: get_resource_string
 *
 * Purpose: Get resource string.
 *
 * Return: PCTSTR
 */
static PCTSTR get_resource_string(UINT uID)
{
	LoadString(ghInstance, uID, szStrResource, MAX_PATH);
	return szStrResource;
}

/*
 * Function: dlg_error_message
 *
 * Purpose: Output error message.
 *
 * Return: NULL
 */
static void dlg_error_message(HWND hDlg, UINT MessageID, UINT TitleID)
{
	TCHAR szErrorMessage[MAX_PATH] = {0};

	lstrcpy(szErrorMessage, get_resource_string(MessageID));
	MessageBox(hDlg, szErrorMessage, get_resource_string(TitleID), MB_ICONERROR | MB_OK);
}


/*
 * Function: output_get_filename
 *
 * Purpose: IDD_OUTPUT: Get preview file name.
 *
 * Return: DWORD
 */
static DWORD output_get_filename(HWND hDlg, PDEVOBJ pdevobj)
{
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;
	DWORD    Ret = SAVE_SUCCESS; // return value
	OPENFILENAME ofn = {0};


	// init OPENFILENAME structure
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hDlg;
	ofn.lpstrFilter = TEXT("bmp (*.bmp)\0*.bmp\0");
	ofn.lpstrFile = pOemPDEV->szPreviewFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_LONGNAMES | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NONETWORKBUTTON;
	ofn.lpstrDefExt = TEXT("bmp");

	if(GetSaveFileName(&ofn))
    {
		SetDlgItemText(hDlg, IDC_FILENAME, ofn.lpstrFile);
    }
	else
	{
		if(CommDlgExtendedError())
			Ret = SAVE_FAILED;
		else
			Ret = SAVE_CANCEL;
	}

	return Ret;
}

/*
 * Function: output_validate_filename
 *
 * Purpose: IDD_OUTPUT: Validate preview filename.
 *
 * Return: BOOL
 */
static BOOL output_validate_filename(HWND hDlg, PDEVOBJ pdevobj)
{
    POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


	// get current file name in edit box
	GetDlgItemText(hDlg, IDC_FILENAME, pOemPDEV->szPreviewFileName, MAX_PATH);

	if(lstrlen(pOemPDEV->szPreviewFileName))
	{
		TCHAR szBuffer[MAX_PATH] = {0};
		PTSTR pFilePart = NULL;

		// get full path name
		GetFullPathName(pOemPDEV->szPreviewFileName, MAX_PATH, szBuffer, &pFilePart);

		// create temp file
		HANDLE hFile = CreateFile(szBuffer, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
        {
			return FALSE;
        }
		else
		{
            // close & delete file
			CloseHandle(hFile);
			DeleteFile(szBuffer);
		}

		// update szPreviewFileName
		lstrcpy(pOemPDEV->szPreviewFileName, szBuffer);
	}
	else
		return FALSE;

	return TRUE;
}
