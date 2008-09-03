/*
 * @file ui/intrface.cpp
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
#include <INITGUID.H>
#include <PRCOMOEM.H>
#include "debug.h"
#include "oem.h"
#include "oemui.h"
#include "devmode.h"
#include "intrface.h"


/*
 *  Internal datas
 */

static long g_cComponents = 0;		// Count of active components
static long g_cServerLocks = 0;		// Count of locks


/*
 *  IOEMUI implementation
 *

/*
 * Function: IOEMUI::~IOEMUI
 *
 * Purpose: Destruction.
 *
 * Return: 
 */
IOEMUI::~IOEMUI()
{
	// make sure that helper interface is released
	if(m_pOEMHelp != NULL)
	{
		m_pOEMHelp->Release();
		m_pOEMHelp = NULL;
	}

	// if this instance of the object is being deleted, then the reference 
	// count should be zero
	assert(0 == m_cRef);
}

/*
 * Function: IOEMUI::QueryInterface
 *
 * Purpose: Returns a pointer to a specified interface on an object to
 *		    which a client currently holds an interface pointer. This
 *		    function must call IUnknown::AddRef on the pointer it returns.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::QueryInterface(
	const IID& iid,
	void** ppv)
{
	VERBOSE(UITEXT("IOEMUI::QueryInterface entry.\r\n"));

	if(iid == IID_IUnknown)
	{
		*ppv = static_cast<IUnknown*>(this);
		VERBOSE(UITEXT("IOEMUI: Return pointer to IUnknown.\r\n"));
	}
	else if(iid == IID_IPrintOemUI)
    {
		*ppv = static_cast<IPrintOemUI*>(this);
		VERBOSE(UITEXT("IOEMUI: Return pointer to IPrintOemUI.\r\n"));
	}
	else
	{
		*ppv = NULL;
		VERBOSE(UITEXT("IOEMUI: No Interface. Return E_NOINTERFACE.\r\n"));
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

/*
 * Function: IOEMUI::AddRef
 *
 * Purpose: Lifetime management.
 *
 * Return: ULONG
 */
ULONG __stdcall IOEMUI::AddRef()
{
	VERBOSE(UITEXT("IOEMUI::AddRef entry.\r\n"));

	return InterlockedIncrement(&m_cRef);
}

/*
 * Function: IOEMUI::Release
 *
 * Purpose: Lifetime management.
 *
 * Return: ULONG
 */
ULONG __stdcall IOEMUI::Release() 
{
	VERBOSE(UITEXT("IOEMUI::Release entry.\r\n"));

	if(InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

/*
 * Function: IOEMUI::CommonUIProp
 *
 * Purpose: Modify an existing printer property sheet page.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::CommonUIProp(
	DWORD			dwMode,
	POEMCUIPPARAM   pOemCUIPParam)
{
	TERSE(UITEXT("IOEMUI::CommonUIProp entry.\r\n"));

	return hrOEMCommonUIProp(dwMode, pOemCUIPParam);
}

/*
 * Function: IOEMUI::DeviceCapabilities
 *
 * Purpose: Specify customized device capabilities.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::DeviceCapabilities(
	POEMUIOBJ   poemuiobj,
	HANDLE      hPrinter,
	PWSTR       pDeviceName,
	WORD        wCapability,
	PVOID       pOutput,
	PDEVMODE    pPublicDM,
	PVOID       pOEMDM,
	DWORD       dwOld,
	DWORD       *dwResult)
{
	VERBOSE(UITEXT("IOEMUI::DeviceCapabilities(%d, %d) entry.\r\n"), wCapability, dwOld);

	return E_NOTIMPL;
}

/*
 * Function: IOEMUI::DevicePropertySheets
 *
 * Purpose: Append a new page to a printer device's printer property sheet.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::DevicePropertySheets(
	PPROPSHEETUI_INFO   pPSUIInfo,
	LPARAM              lParam)
{
	TERSE(UITEXT("IOEMUI::DevicePropertySheets entry.\r\n"));

	return hrOEMDevicePropertySheets(pPSUIInfo, lParam);
}

/*
 * Function: IOEMUI::DevMode
 *
 * Purpose: Perform operations on the oem DEVMODE.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::DevMode(
	DWORD		dwMode,
	POEMDMPARAM pOemDMParam)
{
	VERBOSE(UITEXT("IOEMUI::DevMode(%d) entry.\r\n"), dwMode);

	return hrOEMDevMode(dwMode, pOemDMParam);
}

/*
 * Function: IOEMUI::DevQueryPrintEx
 *
 * Purpose: Help determine if a print job is printable.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::DevQueryPrintEx(
	POEMUIOBJ               poemuiobj,
	PDEVQUERYPRINT_INFO     pDQPInfo,
	PDEVMODE                pPublicDM,
	PVOID                   pOEMDM)
{
	VERBOSE(UITEXT("IOEMUI::DevQueryPrintEx entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUI::DocumentPropertySheets
 *
 * Purpose: Append a new page to a printer device's document property sheet.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::DocumentPropertySheets(
	PPROPSHEETUI_INFO   pPSUIInfo,
	LPARAM              lParam)
{
	TERSE(UITEXT("IOEMUI::DocumentPropertySheets entry.\r\n"));

	return hrOEMDocumentPropertySheets(pPSUIInfo, lParam);
}

/*
 * Function: IOEMUI::DriverEvent
 *
 * Purpose: Additional processing of printer driver events.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::DriverEvent(
	DWORD   dwDriverEvent,
	DWORD   dwLevel,
	LPBYTE  pDriverInfo,
	LPARAM  lParam)
{
	TERSE(UITEXT("IOEMUI::DriverEvent(Event: %d, Level: %d) entry.\r\n"), dwDriverEvent, dwLevel);

	return E_NOTIMPL;
}

/*
 * Function: IOEMUI::FontInstallerDlgProc
 *
 * Purpose: Replaces the Unidrv font installer's user interface.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::FontInstallerDlgProc(
	HWND    hWnd,
	UINT    usMsg,
	WPARAM  wParam,
	LPARAM  lParam) 
{
	VERBOSE(UITEXT("IOEMUI::FontInstallerDlgProc entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUI::GetInfo
 *
 * Purpose: Returns identification information.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::GetInfo(
	DWORD  dwMode,
	PVOID  pBuffer,
	DWORD  cbSize,
	PDWORD pcbNeeded)
{
	VERBOSE(UITEXT("IOEMUI::GetInfo(%d) entry.\r\n"), dwMode);

	// validate parameters
	if((NULL == pcbNeeded)
	   ||
	   (OEMGI_GETSIGNATURE != dwMode && OEMGI_GETVERSION != dwMode && OEMGI_GETPUBLISHERINFO != dwMode)
	  )
	{
		ERR(UITEXT("IOEMUI: GetInfo() exit pcbNeeded is NULL! ERROR_INVALID_PARAMETER\r\n"));

		SetLastError(ERROR_INVALID_PARAMETER);
		return E_FAIL;
	}

	// set expected buffer size and number of bytes written
	*pcbNeeded = sizeof(DWORD);

	// check buffer size is sufficient
	if((cbSize < *pcbNeeded) || (NULL == pBuffer))
	{
		WARNING(UITEXT("IOEMUI: GetInfo() exit insufficient buffer!\r\n"));
		SetLastError(ERROR_INSUFFICIENT_BUFFER);
		return E_FAIL;
	}

	// do action
	switch(dwMode)
	{
	case OEMGI_GETSIGNATURE:
		*(PDWORD)pBuffer = OEM_SIGNATURE;
		break;

	case OEMGI_GETVERSION:
		*(PDWORD)pBuffer = OEM_VERSION;
		break;

	// dwMode not supported
	default:
		WARNING(UITEXT("IOEMUI: GetInfo() exit mode not supported.\r\n"));
		*pcbNeeded = 0;	// set written bytes to zero since nothing was written
		SetLastError(ERROR_NOT_SUPPORTED);
		return E_FAIL;
	}

	return S_OK;
}

/*
 * Function: IOEMUI::PrinterEvent
 *
 * Purpose: Process printer events.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::PrinterEvent(
	PWSTR   pPrinterName,
	INT     iDriverEvent,
	DWORD   dwFlags,
	LPARAM  lParam)
{
	VERBOSE(UITEXT("IOEMUI::PrinterEvent entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUI::PublishDriverInterface
 *
 * Purpose: Obtain the Unidrv's IPrintOemDriverUI interface.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::PublishDriverInterface(
	IUnknown *pIUnknown)
{
	VERBOSE(UITEXT("IOEMUI::PublishDriverInterface entry.\r\n"));

	// need to store pointer to Driver Helper functions, if we already haven't
	if(m_pOEMHelp == NULL)
	{
		HRESULT hResult;

		// get interface to helper functions
		hResult = pIUnknown->QueryInterface(IID_IPrintOemDriverUI, (void** ) &(m_pOEMHelp));

		if(FAILED(hResult))
		{
			// make sure that interface pointer reflects interface query failure
			m_pOEMHelp = NULL;
			return E_FAIL;
		}
	}

	return S_OK;
}

/*
 * Function: IOEMUI::QueryColorProfile
 *
 * Purpose: Specify an ICC profile to use for color management.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::QueryColorProfile(
	HANDLE      hPrinter,
	POEMUIOBJ   poemuiobj,
	PDEVMODE    pPublicDM,
	PVOID       pOEMDM,
	ULONG       ulQueryMode,
	VOID       *pvProfileData,
	ULONG      *pcbProfileData,
	FLONG      *pflProfileData)
{
	VERBOSE(UITEXT("IOEMUI::QueryColorProfile entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUI::UpdateExternalFonts
 *
 * Purpose: Update a printer's Unidrv Font Format file (.uff file).
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::UpdateExternalFonts(
	HANDLE  hPrinter,
	HANDLE  hHeap,
	PWSTR   pwstrCartridges)
{
	VERBOSE(UITEXT("IOEMUI::UpdateExternalFonts entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUI::UpgradePrinter
 *
 * Purpose: Upgrade device option values that are stored in the registry.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUI::UpgradePrinter(
	DWORD   dwLevel,
	PBYTE   pDriverUpgradeInfo)
{
	VERBOSE(UITEXT("IOEMUI::UpgradePrinter entry.\r\n"));

	return E_NOTIMPL;
}


/*
 *  IOEMCF
 */

class IOEMCF : public IClassFactory
{
public:
	// *** IUnknown methods ***
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef)  (THIS);
	STDMETHOD_(ULONG,Release) (THIS);

	// *** IClassFactory methods ***
	STDMETHOD(CreateInstance) (THIS_
			LPUNKNOWN pUnkOuter,
			REFIID riid,
			LPVOID FAR* ppvObject
			);

	STDMETHOD(LockServer)     (THIS_
			BOOL bLock
			);

	// Constructor
	IOEMCF(): m_cRef(1) { };
	~IOEMCF() { };

protected:
	LONG m_cRef;
};


/*
 *  IOEMCF Implementation
 */

/*
 * Function: IOEMCF::QueryInterface
 *
 * Purpose: 
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMCF::QueryInterface(const IID& iid, void** ppv)
{
	VERBOSE(UITEXT("IOEMCF::QueryInterface entry.\r\n"));

	if((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		*ppv = static_cast<IOEMCF*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK ;
}

/*
 * Function: IOEMCF::AddRef
 *
 * Purpose: Lifetime management.
 *
 * Return: ULONG
 */
ULONG __stdcall IOEMCF::AddRef()
{
	VERBOSE(UITEXT("IOEMCF::AddRef entry.\r\n"));

	return InterlockedIncrement(&m_cRef);
}

/*
 * Function: IOEMCF::Release
 *
 * Purpose: Lifetime management.
 *
 * Return: ULONG
 */
ULONG __stdcall IOEMCF::Release()
{
	VERBOSE(UITEXT("IOEMCF::Release entry.\r\n"));

	if(InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

/*
 * Function: IOEMCF::CreateInstance
 *
 * Purpose: 
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMCF::CreateInstance(
	IUnknown* pUnknownOuter,
	const IID& iid,
	void** ppv)
{
	VERBOSE(UITEXT("IOEMCF::CreateInstance entry.\r\n"));

	// cannot aggregate
	if(pUnknownOuter != NULL)
		return CLASS_E_NOAGGREGATION;

	// create component
	IOEMUI* pOEMUI = new IOEMUI;
	if(pOEMUI == NULL)
		return E_OUTOFMEMORY;

	// get the requested interface
	HRESULT hr = pOEMUI->QueryInterface(iid, ppv);

	// release the IUnknown pointer
	// (if QueryInterface failed, component will delete itself)
	pOEMUI->Release();
	return hr;
}

/*
 * Function: IOEMCF::LockServer
 *
 * Purpose: 
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMCF::LockServer(
	BOOL bLock)
{
	VERBOSE(UITEXT("IOEMCF::LockServer entry.\r\n"));

	if(bLock)
		InterlockedIncrement(&g_cServerLocks);
	else
		InterlockedDecrement(&g_cServerLocks);

	return S_OK;
}


/*
 *  Can DLL unload now?
 */
STDAPI DllCanUnloadNow()
{
	VERBOSE(UITEXT("DllCanUnloadNow entry.\r\n"));

	if((g_cComponents == 0) && (g_cServerLocks == 0))
		return S_OK;
	else
		return S_FALSE;
}

/*
 *  Get class factory
 */
STDAPI DllGetClassObject(
	const CLSID& clsid,
	const IID& iid,
	void** ppv)
{
	VERBOSE(UITEXT("DllGetClassObject: Create class factory.\r\n"));


	// can we create this component?
	if(clsid != CLSID_OEMUI)
		return CLASS_E_CLASSNOTAVAILABLE;

	// create class factory
	IOEMCF* pOEMCF = new IOEMCF;   // reference count set to 1 in constructor
	if(pOEMCF == NULL)
		return E_OUTOFMEMORY ;

	// cet requested interface
	HRESULT hr = pOEMCF->QueryInterface(iid, ppv);
	pOEMCF->Release();

	return hr;
}
