/*
 * @file uni/intrface.h
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

#ifndef _INTERFACE_H
#define _INTERFACE_H


/*
 *  IOEMUni
 */
class IOEMUni : public IPrintOemUni
{
public:
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj);
    STDMETHOD_(ULONG,AddRef)  (THIS);
    STDMETHOD_(ULONG,Release) (THIS);


	// *** Implemented methods ***

    STDMETHOD(DevMode) (THIS_ 
			DWORD       dwMode,
			POEMDMPARAM pOemDMParam
			);

	STDMETHOD(DisableDriver) (THIS);

    STDMETHOD(DisablePDEV) (THIS_
			PDEVOBJ         pdevobj
			);

    STDMETHOD(DriverDMS) (THIS_
			PVOID   pDevObj,
			PVOID   pBuffer,
			DWORD   cbSize,
			PDWORD  pcbNeeded
			);

	STDMETHOD(EnableDriver) (THIS_
			DWORD           DriverVersion,
			DWORD           cbSize,
			PDRVENABLEDATA  pded
			);

    STDMETHOD(EnablePDEV) (THIS_
			PDEVOBJ         pdevobj,
			PWSTR           pPrinterName,
			ULONG           cPatterns,
			HSURF          *phsurfPatterns,
			ULONG           cjGdiInfo,
			GDIINFO        *pGdiInfo,
			ULONG           cjDevInfo,
			DEVINFO        *pDevInfo,
			DRVENABLEDATA  *pded,
			OUT PDEVOEM    *pDevOem
			);

	STDMETHOD(GetImplementedMethod) (THIS_
			PSTR pMethodName
			);

    STDMETHOD(GetInfo) (THIS_
			DWORD   dwMode,
			PVOID   pBuffer,
			DWORD   cbSize,
			PDWORD  pcbNeeded
			);

    STDMETHOD(ImageProcessing) (THIS_ 
			PDEVOBJ             pdevobj,
			PBYTE               pSrcBitmap,
			PBITMAPINFOHEADER   pBitmapInfoHeader,
			PBYTE               pColorTable,
			DWORD               dwCallbackID,
			PIPPARAMS           pIPParams,
			OUT PBYTE           *ppbResult
			);

	STDMETHOD(PublishDriverInterface) (THIS_
			IUnknown *pIUnknown
			);

    STDMETHOD(ResetPDEV) (THIS_
			PDEVOBJ        pdevobjOld,
			PDEVOBJ        pdevobjNew
			);


	// *** NOT implemented methods ***

    STDMETHOD(CommandCallback) (THIS_
			PDEVOBJ     pdevobj,
			DWORD       dwCallbackID,
			DWORD       dwCount,
			PDWORD      pdwParams,
			OUT INT     *piResult
			);

    STDMETHOD(Compression) (THIS_
			PDEVOBJ     pdevobj,
			PBYTE       pInBuf,
			PBYTE       pOutBuf,
			DWORD       dwInLen,
			DWORD       dwOutLen,
			OUT INT     *piResult
			);

    STDMETHOD(DownloadCharGlyph) (THIS_
			PDEVOBJ     pdevobj,
			PUNIFONTOBJ pUFObj,
			HGLYPH      hGlyph,
			PDWORD      pdwWidth,
			OUT DWORD   *pdwResult
			);

    STDMETHOD(DownloadFontHeader) (THIS_
			PDEVOBJ     pdevobj,
			PUNIFONTOBJ pUFObj,
			OUT DWORD   *pdwResult
			);

    STDMETHOD(FilterGraphics) (THIS_
			PDEVOBJ     pdevobj,
			PBYTE       pBuf,
			DWORD       dwLen
			);

    STDMETHOD(HalftonePattern) (THIS_
			PDEVOBJ     pdevobj,
			PBYTE       pHTPattern,
			DWORD       dwHTPatternX,
			DWORD       dwHTPatternY,
			DWORD       dwHTNumPatterns,
			DWORD       dwCallbackID,
			PBYTE       pResource,
			DWORD       dwResourceSize
			);

    STDMETHOD(MemoryUsage) (THIS_
			PDEVOBJ         pdevobj,
			POEMMEMORYUSAGE pMemoryUsage
			);

    STDMETHOD(OutputCharStr) (THIS_
			PDEVOBJ     pdevobj,
			PUNIFONTOBJ pUFObj,
			DWORD       dwType,
			DWORD       dwCount,
			PVOID       pGlyph
			);

    STDMETHOD(SendFontCmd) (THIS_
			PDEVOBJ      pdevobj,
			PUNIFONTOBJ  pUFObj,
			PFINVOCATION pFInv
			);

    STDMETHOD(TextOutAsBitmap)(THIS_
			SURFOBJ    *pso,
			STROBJ     *pstro,
			FONTOBJ    *pfo,
			CLIPOBJ    *pco,
			RECTL      *prclExtra,
			RECTL      *prclOpaque,
			BRUSHOBJ   *pboFore,
			BRUSHOBJ   *pboOpaque,
			POINTL     *pptlOrg,
			MIX         mix
			);

    STDMETHOD(TTDownloadMethod) (THIS_
			PDEVOBJ     pdevobj,
			PUNIFONTOBJ pUFObj,
			OUT DWORD   *pdwResult
			);

    STDMETHOD(TTYGetInfo) (THIS_
			PDEVOBJ     pdevobj,
			DWORD       dwInfoIndex,
			PVOID       pOutputBuf,
			DWORD       dwSize,
			DWORD       *pcbcNeeded
			);

    
    IOEMUni() { m_cRef = 1; m_pOEMHelp = NULL; };
    ~IOEMUni();

protected:
    LONG                m_cRef;
    IPrintOemDriverUni* m_pOEMHelp;
};


#endif // !_INTERFACE_H
