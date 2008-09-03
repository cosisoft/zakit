/*
 * @file uni/intrface.cpp
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
#include "oemuni.h"
#include "devmode.h"
#include "name.h"
#include "enable.h"
#include "imgproc.h"
#include "intrface.h"


/*
 *  Internal datas
 */

static long g_cComponents = 0;     // Count of active components
static long g_cServerLocks = 0;    // Count of locks


/*
 *  IOEMUni implementation
 */

/*
 * Function: IOEMUni::~IOEMUni
 *
 * Purpose: Destruction.
 *
 * Return: NULL
 */
IOEMUni::~IOEMUni()
{
    // make sure that helper interface is released
    if(NULL != m_pOEMHelp)
    {
        m_pOEMHelp->Release();
        m_pOEMHelp = NULL;
    }

    // If this instance of the object is being deleted, then the reference 
    // count should be zero.
    assert(0 == m_cRef);
}

/*
 * Function: IOEMUni::QueryInterface
 *
 * Purpose: 
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::QueryInterface(const IID& iid, void** ppv)
{
	VERBOSE(UNITEXT("IOEMUni::QueryInterface entry.\r\n"));

    if(iid == IID_IUnknown)
    {
        *ppv = static_cast<IUnknown*>(this); 
        VERBOSE(UNITEXT("IOEMUni: QueryInterface IUnknown.\r\n")); 
    }
    else if(iid == IID_IPrintOemUni)
    {
        *ppv = static_cast<IPrintOemUni*>(this);
        VERBOSE(UNITEXT("IOEMUni: QueryInterface IPrintOemUni.\r\n")); 
    }
    else
    {
        *ppv = NULL;
        VERBOSE(UNITEXT("IOEMUni: QueryInterface NULL. Returning E_NOINTERFACE.\r\n")); 
        return E_NOINTERFACE;
    }
    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return S_OK;
}

/*
 * Function: IOEMUni::AddRef
 *
 * Purpose: Lifetime management.
 *
 * Return: ULONG
 */
ULONG __stdcall IOEMUni::AddRef()
{
    VERBOSE(UNITEXT("IOEMUni::AddRef() entry.\r\n"));

    return InterlockedIncrement(&m_cRef);
}

/*
 * Function: IOEMUni::Release
 *
 * Purpose: Lifetime management.
 *
 * Return: ULONG
 */
ULONG __stdcall IOEMUni::Release() 
{
    VERBOSE(UNITEXT("IOEMUni::Release() entry.\r\n"));

    if(InterlockedDecrement(&m_cRef) == 0)
    {
        delete this;
        return 0;
    }
    return m_cRef;
}


// *** Implemented methods ***

/*
 * Function: IOEMUni::DevMode
 *
 * Purpose: Performs operations on OEM DEVMODE members.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::DevMode(
    DWORD       dwMode,
    POEMDMPARAM pOemDMParam)
{   
	VERBOSE(UNITEXT("IOEMUni:DevMode(%d) entry.\n"), dwMode);

	return hrOEMDevMode(dwMode, pOemDMParam);
}

/*
 * Function: IOEMUni::DisableDriver
 *
 * Purpose: Free resources that were allocated by EnableDriver method.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::DisableDriver(VOID)
{
	TERSE(UNITEXT("IOEMUni::DisaleDriver() entry.\r\n"));

	OEMDisableDriver();

	// release reference to Unidrv's interface
	if(this->m_pOEMHelp)
    {
		this->m_pOEMHelp->Release();
		this->m_pOEMHelp = NULL;
    }

    return S_OK;
}

/*
 * Function: IOEMUni::DisablePDEV
 *
 * Purpose: Delete the oem PDEV structure that was allocated by EnablePDEV method.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::DisablePDEV(
    PDEVOBJ         pdevobj)
{
	TERSE(UNITEXT("IOEMUni::DisablePDEV() entry.\r\n"));

	OEMDisablePDEV(pdevobj);

	return S_OK;
}

/*
 * Function: IOEMUni::DriverDMS
 *
 * Purpose: Allows rendering plug-in for Unidrv to indicate that it will
 *		    use a device-managed drawing surface.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::DriverDMS(
    PVOID   pDevObj,
    PVOID   pBuffer,
    DWORD   cbSize,
    PDWORD  pcbNeeded)
{
	VERBOSE(UNITEXT("IOEMUni::DriverDMS() entry.\r\n"));

	return S_OK;
}

/*
 * Function: IOEMUni::EnableDriver
 *
 * Purpose: Hook out some DDI functions.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::EnableDriver(
	DWORD          dwDriverVersion,
	DWORD          cbSize,
	PDRVENABLEDATA pded)
{
	TERSE(UNITEXT("IOEMUni::EnableDriver() entry.\r\n"));

	OEMEnableDriver(dwDriverVersion, cbSize, pded);

	// Even if nothing is done, need to return S_OK so 
	// that DisableDriver() will be called, which releases
	// the reference to the Printer Driver's interface.
	return S_OK;
}

/*
 * Function: IOEMUni::EnablePDEV
 *
 * Purpose: Create oem PDEV structure.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::EnablePDEV(
    PDEVOBJ         pdevobj,
    PWSTR           pPrinterName,
    ULONG           cPatterns,
    HSURF          *phsurfPatterns,
    ULONG           cjGdiInfo,
    GDIINFO        *pGdiInfo,
    ULONG           cjDevInfo,
    DEVINFO        *pDevInfo,
    DRVENABLEDATA  *pded,
    OUT PDEVOEM    *pDevOem)
{
	TERSE(UNITEXT("IOEMUni::EnablePDEV() entry.\r\n"));

	*pDevOem = OEMEnablePDEV(pdevobj,
				 			 pPrinterName,
							 cPatterns,
							 phsurfPatterns,
							 cjGdiInfo,
							 pGdiInfo,
							 cjDevInfo,
							 pDevInfo,
							 pded,
							 this->m_pOEMHelp);

	return (NULL != *pDevOem ? S_OK : E_FAIL);
}

/*
 * Function: IOEMUni::GetImplementedMethod
 *
 * Purpose: Used by Unidrv to determine which IPrintOemUni interface methods
 *		    have been implemented by a rendering plug-in.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::GetImplementedMethod(
	PSTR pMethodName)
{
	VERBOSE(UNITEXT("IOEMUni::GetImplementedMethod() entry.\r\n"));


	HRESULT Result = S_FALSE;

	switch(*pMethodName)
    {
	case 'I':
		if(!strcmp(NAME_ImageProcessing, pMethodName))
			Result = S_OK;

		break;
	}

	return Result;
}

/*
 * Function: IOEMUni::GetInfo
 *
 * Purpose: Returns identification information.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::GetInfo (
    DWORD   dwMode,
    PVOID   pBuffer,
    DWORD   cbSize,
    PDWORD  pcbNeeded)
{
	VERBOSE(UNITEXT("IOEMUni::GetInfo(%d) entry.\r\n"), dwMode);

    // validate parameters
    if((NULL == pcbNeeded)
       ||
       ((OEMGI_GETSIGNATURE != dwMode) && (OEMGI_GETVERSION != dwMode) && (OEMGI_GETPUBLISHERINFO != dwMode))
      )
    {
        WARNING(UNITEXT("IOEMUni: GetInfo() exit pcbNeeded is NULL! ERROR_INVALID_PARAMETER.\r\n"));
        SetLastError(ERROR_INVALID_PARAMETER);
        return E_FAIL;
    }

    // set expected buffer size
    if(OEMGI_GETPUBLISHERINFO != dwMode)
    {
        *pcbNeeded = sizeof(DWORD);
    }
    else
    {
        *pcbNeeded = sizeof(PUBLISHERINFO);
        return E_FAIL;
    }

    // check buffer size is sufficient
    if((cbSize < *pcbNeeded) || (NULL == pBuffer))
    {
        VERBOSE(UNITEXT("IOEMUni: GetInfo() exit insufficient buffer!\r\n"));
        SetLastError(ERROR_INSUFFICIENT_BUFFER);
        return E_FAIL;
    }

    switch(dwMode)
    {
        // DLL Signature
        case OEMGI_GETSIGNATURE:
            *(PDWORD)pBuffer = OEM_SIGNATURE;
            break;

        // OEM DLL version
        case OEMGI_GETVERSION:
            *(PDWORD)pBuffer = OEM_VERSION;
            break;

        case OEMGI_GETPUBLISHERINFO:
            // fall through to default case

        // dwMode not supported
        default:
            // set written bytes to zero since nothing was written
            WARNING(UNITEXT("IOEMUni: GetInfo() exit mode not supported.\r\n"));
            *pcbNeeded = 0;
            SetLastError(ERROR_NOT_SUPPORTED);
            return E_FAIL;
    }

    return S_OK;
}

/*
 * Function: IOEMUni::ImageProcessing
 *
 * Purpose: Perform customized format convertion or halftoning.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::ImageProcessing(
	PDEVOBJ             pdevobj,
	PBYTE               pSrcBitmap,
	PBITMAPINFOHEADER   pBitmapInfoHeader,
	PBYTE               pColorTable,
	DWORD               dwCallbackID,
	PIPPARAMS           pIPParams,
	OUT PBYTE           *ppbResult)
{
	TERSE(UNITEXT("IOEMUni::ImageProcessing() entry.\r\n"));


	POEMPDEV pOemPDEV = (POEMPDEV)pdevobj->pdevOEM;


	// record information for processed bands
	if(!pOemPDEV->DIBInfo.bPageStarted)
	{
		pOemPDEV->DIBInfo.DIBSize.cx = pBitmapInfoHeader->biWidth;
		pOemPDEV->DIBInfo.DIBSize.cy = 0;

        if(dwCallbackID == 24) // 24bpp
            pOemPDEV->DIBInfo.WidthBytes = (pOemPDEV->DIBInfo.DIBSize.cx * 24 + 31 ) / 32 * 4;
        else // 1bpp
		    pOemPDEV->DIBInfo.WidthBytes = (pOemPDEV->DIBInfo.DIBSize.cx + 31) / 32 * 4;

		pOemPDEV->DIBInfo.dwDIBTotalBytes = 0;

		pOemPDEV->DIBInfo.bPageStarted = TRUE;
	}

    // dump bits
    dump(pdevobj, pSrcBitmap, pBitmapInfoHeader, pIPParams);

	// we do NOT need sending data to spooler or moving cursor,
	// so there is NO need to call IPrintOemDriverUni::DrvWriteSpoolBuf,
	// IPrintOemDriverUni::DrvXMoveTo or IPrintOemDriverUni::DrvYMoveTo.

	// it is very important to return bmp pointer to Unidrv,
	// otherwise, NextBand will return FALSE.
	*ppbResult = pSrcBitmap;

	return S_OK;
}

/*
 * Function: IOEMUni::PublishDriverInterface
 *
 * Purpose: Obtain the Unidrv driver's IPrintOemDriverUni interface.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::PublishDriverInterface(
	IUnknown *pIUnknown)
{
	VERBOSE(UNITEXT("IOEMUni::PublishDriverInterface() entry.\r\n"));

	// need to store pointer to Driver Helper functions, if we already haven't
	if(this->m_pOEMHelp == NULL)
	{
		HRESULT hResult;

		// get Interface to Helper Functions
		hResult = pIUnknown->QueryInterface(IID_IPrintOemDriverUni, (void** ) &(this->m_pOEMHelp));

		if(FAILED(hResult))
		{
			// make sure that interface pointer reflects interface query failure
			this->m_pOEMHelp = NULL;
			return E_FAIL;
		}
	}

	return S_OK;
}

/*
 * Function: IOEMUni::ResetPDEV
 *
 * Purpose: Reset PDEV structure.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::ResetPDEV(
	PDEVOBJ        pdevobjOld,
	PDEVOBJ        pdevobjNew)
{
	TERSE(UNITEXT("IOEMUni::ResetPDEV() entry.\r\n"));

	BOOL bResult = OEMResetPDEV(pdevobjOld, pdevobjNew);

	return (bResult ? S_OK : E_FAIL);
}


// *** NOT implemented methods ***

/*
 * Function: IOEMUni::CommandCallback
 *
 * Purpose: Dynamically generate printer commands for printers.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::CommandCallback(
    PDEVOBJ     pdevobj,
    DWORD       dwCallbackID,
    DWORD       dwCount,
    PDWORD      pdwParams,
    OUT INT     *piResult)
{
	VERBOSE(UNITEXT("IOEMUni::CommandCallback() entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUni::Compression
 *
 * Purpose: Provide a customized bitmap compression method.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::Compression(
    PDEVOBJ     pdevobj,
    PBYTE       pInBuf,
    PBYTE       pOutBuf,
    DWORD       dwInLen,
    DWORD       dwOutLen,
    OUT INT     *piResult)
{
	VERBOSE(UNITEXT("IOEMUni::Compression() entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUni::DownloadCharGlyph
 *
 * Purpose: Send a character glyph for a specified soft font to the printer.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::DownloadCharGlyph(
    PDEVOBJ     pdevobj,
    PUNIFONTOBJ pUFObj,
    HGLYPH      hGlyph,
    PDWORD      pdwWidth,
    OUT DWORD   *pdwResult) 
{
	VERBOSE(UNITEXT("IOEMUni::DownloadCharGlyph() entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUni::DownloadFontHeader
 *
 * Purpose: Send a font's header information to printer.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::DownloadFontHeader(
    PDEVOBJ     pdevobj,
    PUNIFONTOBJ pUFObj,
    OUT DWORD   *pdwResult) 
{
	VERBOSE(UNITEXT("IOEMUni::DownloadFontHeader() entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUni::FilterGraphics
 *
 * Purpose: Modify scan line data and send it to the spooler.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::FilterGraphics(
    PDEVOBJ     pdevobj,
    PBYTE       pBuf,
    DWORD       dwLen)
{
	VERBOSE(UNITEXT("IOEMUni::FilterGraphis() entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUni::HalftonePattern
 *
 * Purpose: Create or modify a halftone pattern before it is used in
 *		   a halftoning operation.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::HalftonePattern(
    PDEVOBJ     pdevobj,
    PBYTE       pHTPattern,
    DWORD       dwHTPatternX,
    DWORD       dwHTPatternY,
    DWORD       dwHTNumPatterns,
    DWORD       dwCallbackID,
    PBYTE       pResource,
    DWORD       dwResourceSize)
{
	VERBOSE(UNITEXT("IOEMUni::HalftonePattern() entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUni::MemoryUsage
 *
 * Purpose: Specify the amount of memory required for use by
 *		    IPrintOemUni::ImageProcessing method.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::MemoryUsage(
    PDEVOBJ         pdevobj,
    POEMMEMORYUSAGE pMemoryUsage)
{
	VERBOSE(UNITEXT("IOEMUni::MemoryUsage() entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUni::OutputCharStr
 *
 * Purpose: Control the printing of font glyphs.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::OutputCharStr(
    PDEVOBJ     pdevobj,
    PUNIFONTOBJ pUFObj,
    DWORD       dwType,
    DWORD       dwCount,
    PVOID       pGlyph) 
{
	VERBOSE(UNITEXT("IOEMUni::OutputCharStr() entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUni::SendFontCmd
 *
 * Purpose: Modify font selection command and then send it to printer.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::SendFontCmd(
    PDEVOBJ      pdevobj,
    PUNIFONTOBJ  pUFObj,
    PFINVOCATION pFInv) 
{
	VERBOSE(UNITEXT("IOEMUni::SendFontCmd() entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUni::TextOutAsBitmap
 *
 * Purpose: Create a bitmap image of a text string, in case a downloadable
 *		    font is not available.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::TextOutAsBitmap(
    SURFOBJ    *pso,
    STROBJ     *NAME_o,
    FONTOBJ    *pfo,
    CLIPOBJ    *pco,
    RECTL      *prclExtra,
    RECTL      *prclOpaque,
    BRUSHOBJ   *pboFore,
    BRUSHOBJ   *pboOpaque,
    POINTL     *pptlOrg,
    MIX         mix)
{
	VERBOSE(UNITEXT("IOEMUni::TextOutAsBitmap() entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUni::TTDownloadMethod
 *
 * Purpose: Indicate the format that Unidrv should use for a specified
 *		    TrueType soft font.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::TTDownloadMethod(
    PDEVOBJ     pdevobj,
    PUNIFONTOBJ pUFObj,
    OUT DWORD   *pdwResult) 
{
	VERBOSE(UNITEXT("IOEMUni::TTDownloadMethod() entry.\r\n"));

	return E_NOTIMPL;
}

/*
 * Function: IOEMUni::TTYGetInfo
 *
 * Purpose: Supply Unidrv with information relevant to text-only printers.
 *
 * Return: HRESULT
 */
HRESULT __stdcall IOEMUni::TTYGetInfo(
    PDEVOBJ     pdevobj,
    DWORD       dwInfoIndex,
    PVOID       pOutputBuf,
    DWORD       dwSize,
    DWORD       *pcbcNeeded)
{
	VERBOSE(UNITEXT("IOEMUni::TTYGetInfo() entry.\r\n"));

	return E_NOTIMPL;
}


/*
 *  OEM class factory
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
 * Class factory implementation
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
	VERBOSE(UNITEXT("IOEMCF::QueryInterface entry.\r\n"));

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
    return S_OK;
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
	VERBOSE(UNITEXT("IOEMCF::AddRef entry.\r\n"));

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
	VERBOSE(UNITEXT("IOEMCF::Release entry.\r\n"));

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
	VERBOSE(UNITEXT("IOEMCF::CreateInstance entry.\r\n"));

    // cannot aggregate
    if(pUnknownOuter != NULL)
        return CLASS_E_NOAGGREGATION;

    // create component
    IOEMUni* pOEMUni = new IOEMUni;
    if(pOEMUni == NULL)
        return E_OUTOFMEMORY;

    // get the requested interface
    HRESULT hr = pOEMUni->QueryInterface(iid, ppv);

    // release the IUnknown pointer
    // (If QueryInterface failed, component will delete itself.)
    pOEMUni->Release();
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
	VERBOSE(UNITEXT("IOEMCF::LockServer entry.\r\n"));

    if(bLock)
        InterlockedIncrement(&g_cServerLocks); 
    else
        InterlockedDecrement(&g_cServerLocks);

    return S_OK;
}


/*
 *  Registration functions
 */

/*
 * Function: IOEMCF::DllCanUnloadNow
 *
 * Purpose: Can DLL unload now?
 *
 * Return: 
 */
STDAPI DllCanUnloadNow()
{
    VERBOSE(UNITEXT("DllCanUnloadNow entry.\r\n"));

    if((g_cComponents == 0) && (g_cServerLocks == 0))
        return S_OK;
    else
        return S_FALSE;
}

/*
 * Function: IOEMCF::DllGetClassObject
 *
 * Purpose: Get class factory.
 *
 * Return: 
 */
STDAPI DllGetClassObject(
	const CLSID& clsid,
	const IID& iid,
	void** ppv)
{
    VERBOSE(UNITEXT("DllGetClassObject: Create class factory.\r\n"));

    // can we create this component?
    if(clsid != CLSID_OEMRENDER)
        return CLASS_E_CLASSNOTAVAILABLE;

    // create class factory
    IOEMCF* pOEMCF = new IOEMCF;  // reference count set to 1 in constructor
    if(pOEMCF == NULL)
        return E_OUTOFMEMORY;

    // get requested interface
    HRESULT hr = pOEMCF->QueryInterface(iid, ppv);
    pOEMCF->Release();

    return hr;
}
