/*
 * @file ui/pageclbk.cpp Callback function for property pages.
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
#include "oemui.h"
#include "devmode.h"
#include "resource.h"
#include "pageclbk.h"


/*
 *  Internal prototypes
 */

static void init_custom(HWND hDlg, PDIALOGINFO pdi);
static void custom_init_color_settings(HWND hDlg, PDIALOGINFO pdi);
static void custom_enable_settings(HWND hDlg, PDIALOGINFO pdi);


/*
 *  Implementations
 */

/*
 * Function: DlgCustomProc
 *
 * Purpose: Call back function for IDD_CUSTOM dialog.
 *
 * Return: BOOL
 */
BOOL CALLBACK DlgCustomProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PDIALOGINFO pdi = NULL;


	switch(uMsg)
	{
	case WM_INITDIALOG:
		pdi = (PDIALOGINFO)(((LPPROPSHEETPAGE)lParam)->lParam);
		SetWindowLong(hDlg, DWL_USER, (LONG)pdi);

		init_custom(hDlg, pdi);
		break;

	case WM_COMMAND:
		pdi = (PDIALOGINFO)GetWindowLong(hDlg, DWL_USER);

		switch(LOWORD(wParam))
		{
		// black ~ cyan, check bitmaped button
		case IDC_BITMAPED_BLACK:
		case IDC_BITMAPED_RED:
		case IDC_BITMAPED_GREEN:
		case IDC_BITMAPED_BLUE:
		case IDC_BITMAPED_YELLOW:
		case IDC_BITMAPED_MAGENTA:
		case IDC_BITMAPED_CYAN:
            pdi->pOemDev->dmPenInfo[LOWORD(wParam) - IDC_BITMAPED_BLACK].pibBitmaped ^= 1;
			PropSheet_Changed(GetParent(hDlg), hDlg);
			break;

		// black ~ cyan, check path button
		case IDC_PATH_BLACK:
		case IDC_PATH_RED:
		case IDC_PATH_GREEN:
		case IDC_PATH_BLUE:
		case IDC_PATH_YELLOW:
		case IDC_PATH_MAGENTA:
		case IDC_PATH_CYAN:
            pdi->pOemDev->dmPenInfo[LOWORD(wParam) - IDC_PATH_BLACK].pibPath ^= 1;
			PropSheet_Changed(GetParent(hDlg), hDlg);
			break;

		// black ~ cyan, check text button
		case IDC_TEXT_BLACK:
		case IDC_TEXT_RED:
		case IDC_TEXT_GREEN:
		case IDC_TEXT_BLUE:
		case IDC_TEXT_YELLOW:
		case IDC_TEXT_MAGENTA:
		case IDC_TEXT_CYAN:
            pdi->pOemDev->dmPenInfo[LOWORD(wParam) - IDC_TEXT_BLACK].pibText ^= 1;
			PropSheet_Changed(GetParent(hDlg), hDlg);
			break;

		default:
			return FALSE;
		}
		break;

	// property sheet notification messages
	case WM_NOTIFY:
		{
			NMHDR FAR * lpnmhdr = (NMHDR FAR *)lParam;
			pdi = (PDIALOGINFO)GetWindowLong(hDlg, DWL_USER);

			switch(lpnmhdr->code)
			{
            // validate parameters
			case PSN_KILLACTIVE:
				SetWindowLong(hDlg, DWL_MSGRESULT, FALSE);
				break;

			// when page is to be active, perform initialization here
			case PSN_SETACTIVE:
                if(pdi->pOemDev->dmOldColor != pdi->pPubDev->dmColor) // color mode changed
                {
                    custom_enable_settings(hDlg, pdi);

                    pdi->pOemDev->dmOldColor = pdi->pPubDev->dmColor;
                }
				break;

			// handel ok or apply button
			case PSN_APPLY:
				pdi->bOK = TRUE;
				SetWindowLong(hDlg, DWL_MSGRESULT, PSNRET_NOERROR);
				break;

			// handel cancel button
			case PSN_RESET:
				pdi->bOK = FALSE;
				break;

			// Handle help request
			case PSN_HELP:
				break;
			}
		}
		break;

	case WM_HELP:
		break;

	case WM_CONTEXTMENU:
		break;
		
	default:
		return FALSE;
	}

	return TRUE;
}


/*
 * Function: DlgAboutProc
 *
 * Purpose: Call back function for IDD_ABOUT dialog.
 *
 * Return: BOOL
 */
BOOL CALLBACK DlgAboutProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		break;

	// property sheet notification messages
	case WM_NOTIFY:
		{
			NMHDR FAR * lpnmhdr = (NMHDR FAR *)lParam;

			switch(lpnmhdr->code)
			{
			// validate dialog data
			case PSN_KILLACTIVE:
				SetWindowLong(hDlg, DWL_MSGRESULT, FALSE);
				break;

			// when page is to be active, perform initialization here
			case PSN_SETACTIVE:
				break;

			// handel ok or apply button
			case PSN_APPLY:
				SetWindowLong(hDlg, DWL_MSGRESULT, PSNRET_NOERROR);
				break;

			// handel cancel button
			case PSN_RESET:
				break;

			// handle help request
			case PSN_HELP:
				break;
			}
		}
		break;

	case WM_HELP:
		break;

	case WM_CONTEXTMENU:
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


/*
 *  Internal implementation
 */

/*
 * Function: init_custom
 *
 * Purpose: IDD_CUSTOM: Initialize IDD_CUSTOM dialog page.
 *
 * Return: NULL
 */
static void init_custom(HWND hDlg, PDIALOGINFO pdi)
{
	// init color settings
    custom_init_color_settings(hDlg, pdi);
    custom_enable_settings(hDlg, pdi);
}

/*
 * Function: custom_init_color_settings
 *
 * Purpose: IDD_CUSTOM: Initialize color settings.
 *
 * Return: NULL
 */
static void custom_init_color_settings(HWND hDlg, PDIALOGINFO pdi)
{
    for(short Loop = 0; Loop < NUM_COLORS; Loop++)
    {
        CheckDlgButton(hDlg, IDC_BITMAPED_BLACK + Loop, pdi->pOemDev->dmPenInfo[Loop].pibBitmaped);
        CheckDlgButton(hDlg, IDC_PATH_BLACK + Loop, pdi->pOemDev->dmPenInfo[Loop].pibPath);
        CheckDlgButton(hDlg, IDC_TEXT_BLACK + Loop, pdi->pOemDev->dmPenInfo[Loop].pibText);
    }
}

/*
 * Function: custom_enable_settings
 *
 * Purpose: IDD_CUSTOM: Enable/disable controls.
 *
 * Return: NULL
 */
static void custom_enable_settings(HWND hDlg, PDIALOGINFO pdi)
{
    for(short Loop = PCOLOR_RED; Loop < NUM_COLORS; Loop++)
    {
        if(pdi->pPubDev->dmColor == DMCOLOR_MONOCHROME) // mono
        {
            ShowWindow(GetDlgItem(hDlg, IDC_COLOR_BLACK + Loop), SW_HIDE);
            EnableWindow(GetDlgItem(hDlg, IDC_BITMAPED_BLACK + Loop), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_PATH_BLACK + Loop), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_TEXT_BLACK + Loop), FALSE);
        }
        else // color
        {
            ShowWindow(GetDlgItem(hDlg, IDC_COLOR_BLACK + Loop), SW_SHOW);
            EnableWindow(GetDlgItem(hDlg, IDC_BITMAPED_BLACK + Loop), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_PATH_BLACK + Loop), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_TEXT_BLACK + Loop), TRUE);
        }
    }
}
