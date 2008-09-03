/*
 * @file ui/intrface.h
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

#ifndef _INTRFACE_H
#define _INTRFACE_H


/*
 *  IOEMUI
 */
class IOEMUI: public IPrintOemUI
{
public:
	// *** IUnknown methods ***
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef)  (THIS);
	STDMETHOD_(ULONG,Release) (THIS);


	// *** IPrintOemUI methods ***

	STDMETHOD(CommonUIProp) (THIS_
			DWORD  dwMode,
			POEMCUIPPARAM   pOemCUIPParam
			);

	STDMETHOD(DeviceCapabilities) (THIS_
			POEMUIOBJ   poemuiobj,
			HANDLE      hPrinter,
			PWSTR       pDeviceName,
			WORD        wCapability,
			PVOID       pOutput,
			PDEVMODE    pPublicDM,
			PVOID       pOEMDM,
			DWORD       dwOld,
			DWORD       *dwResult
			);

	STDMETHOD(DevicePropertySheets) (THIS_
			PPROPSHEETUI_INFO   pPSUIInfo,
			LPARAM              lParam
			);

	STDMETHOD(DevMode) (THIS_
			DWORD  dwMode,
			POEMDMPARAM pOemDMParam
			);

	STDMETHOD(DevQueryPrintEx) (THIS_
			POEMUIOBJ               poemuiobj,
			PDEVQUERYPRINT_INFO     pDQPInfo,
			PDEVMODE                pPublicDM,
			PVOID                   pOEMDM
			);

	STDMETHOD(DocumentPropertySheets) (THIS_
			PPROPSHEETUI_INFO   pPSUIInfo,
			LPARAM              lParam
			);

	STDMETHOD(DriverEvent)(THIS_
			DWORD   dwDriverEvent,
			DWORD   dwLevel,
			LPBYTE  pDriverInfo,
			LPARAM  lParam
			);

	STDMETHOD(FontInstallerDlgProc) (THIS_ 
			HWND    hWnd,
			UINT    usMsg,
			WPARAM  wParam,
			LPARAM  lParam
			);

	STDMETHOD(GetInfo) (THIS_
			DWORD  dwMode,
			PVOID  pBuffer,
			DWORD  cbSize,
			PDWORD pcbNeeded
			);

	STDMETHOD(PrinterEvent) (THIS_
			PWSTR   pPrinterName,
			INT     iDriverEvent,
			DWORD   dwFlags,
			LPARAM  lParam
			);

	STDMETHOD(PublishDriverInterface)(THIS_
			IUnknown *pIUnknown
			);

	STDMETHOD(QueryColorProfile) (THIS_
			HANDLE      hPrinter,
			POEMUIOBJ   poemuiobj,
			PDEVMODE    pPublicDM,
			PVOID       pOEMDM,
			ULONG       ulReserved,
			VOID       *pvProfileData,
			ULONG      *pcbProfileData,
			FLONG      *pflProfileData
			);

	STDMETHOD(UpdateExternalFonts) (THIS_
			HANDLE  hPrinter,
			HANDLE  hHeap,
			PWSTR   pwstrCartridges
			);

	STDMETHOD(UpgradePrinter) (THIS_
			DWORD   dwLevel,
			PBYTE   pDriverUpgradeInfo
			);
 

	IOEMUI() { m_cRef = 1; m_pOEMHelp = NULL; };
	~IOEMUI();

protected:
	LONG				m_cRef;
	IPrintOemDriverUI*	m_pOEMHelp;
};


#endif // !_INTRFACE_H
