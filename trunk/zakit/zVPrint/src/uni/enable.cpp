/*
 * @file uni/enable.cpp Enable function.
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
#include "ddiblt.h"
#include "ddicntl.h"
#include "ddipath.h"
#include "dditext.h"
#include "enable.h"


/*
 *  Internal constants
 */

/*
 * Warning: the following array order must match the 
 *          order in enum ENUMHOOKS.
 */
static const DRVFN OEMHookFuncs[] =
{
//    { INDEX_DrvRealizeBrush,        (PFN) OEMRealizeBrush        },
//    { INDEX_DrvDitherColor,         (PFN) OEMDitherColor         },
//    { INDEX_DrvCopyBits,            (PFN) OEMCopyBits            },
    { INDEX_DrvBitBlt,              (PFN) OEMBitBlt              },
    { INDEX_DrvStretchBlt,          (PFN) OEMStretchBlt          },
//    { INDEX_DrvStretchBltROP,       (PFN) OEMStretchBltROP       },
//    { INDEX_DrvPlgBlt,              (PFN) OEMPlgBlt              },
//    { INDEX_DrvTransparentBlt,      (PFN) OEMTransparentBlt      },
//    { INDEX_DrvAlphaBlend,          (PFN) OEMAlphaBlend          },
//    { INDEX_DrvGradientFill,        (PFN) OEMGradientFill        },
    { INDEX_DrvTextOut,             (PFN) OEMTextOut             },
    { INDEX_DrvStrokePath,          (PFN) OEMStrokePath          },
//    { INDEX_DrvFillPath,            (PFN) OEMFillPath            },
//    { INDEX_DrvStrokeAndFillPath,   (PFN) OEMStrokeAndFillPath   },
//    { INDEX_DrvPaint,               (PFN) OEMPaint               },
//    { INDEX_DrvLineTo,              (PFN) OEMLineTo              },
    { INDEX_DrvStartPage,           (PFN) OEMStartPage           },
    { INDEX_DrvSendPage,            (PFN) OEMSendPage            },
//    { INDEX_DrvEscape,              (PFN) OEMEscape              },
    { INDEX_DrvStartDoc,            (PFN) OEMStartDoc            },
    { INDEX_DrvEndDoc,              (PFN) OEMEndDoc              },
    { INDEX_DrvNextBand,            (PFN) OEMNextBand            },
    { INDEX_DrvStartBanding,        (PFN) OEMStartBanding        }
//    { INDEX_DrvQueryFont,           (PFN) OEMQueryFont           },
//    { INDEX_DrvQueryFontTree,       (PFN) OEMQueryFontTree       },
//    { INDEX_DrvQueryFontData,       (PFN) OEMQueryFontData       },
//    { INDEX_DrvQueryAdvanceWidths,  (PFN) OEMQueryAdvanceWidths  },
//    { INDEX_DrvFontManagement,      (PFN) OEMFontManagement      },
//    { INDEX_DrvGetGlyphMode,        (PFN) OEMGetGlyphMode        }
};


/*
 *  Implementations
 */

/*
 * Function: OEMEnableDriver
 *
 * Purpose: Hook out some DDI functions.
 *
 * Return: BOOL
 */
BOOL APIENTRY OEMEnableDriver(
	DWORD dwOEMintfVersion,
	DWORD dwSize,
	PDRVENABLEDATA pded)
{
	// list DDI functions that are hooked
	pded->iDriverVersion = DDI_DRIVER_VERSION_NT5;
	pded->c = sizeof(OEMHookFuncs) / sizeof(DRVFN);
	pded->pdrvfn = (DRVFN *)OEMHookFuncs;

	return TRUE;
}

/*
 * Function: OEMDisableDriver
 *
 * Purpose: Free resources allocated in OEMEnableDriver function.
 *
 * Return: NULL
 */
VOID APIENTRY OEMDisableDriver()
{
	// do nothing
}

/*
 * Function: OEMEnablePDEV
 *
 * Purpose: Create oem PDEV structure.
 *
 * Action: Allocate memory for oem PDEV structure, initialize pointer to
 *		   Unidrv DDI functions, set helper function pointer and create heap.
 *
 * Return: if successed, return the pointer to oem PDEV structure;
 *		   otherwise, return NULL
 */
PDEVOEM APIENTRY OEMEnablePDEV(
    PDEVOBJ				pdevobj,
    PWSTR				pPrinterName,
    ULONG				cPatterns,
    HSURF			   *phsurfPatterns,
    ULONG				cjGdiInfo,
    GDIINFO            *pGdiInfo,
    ULONG				cjDevInfo,
    DEVINFO            *pDevInfo,
    DRVENABLEDATA	   *pded,
	IPrintOemDriverUni *pOEMHelp)
{
    POEMPDEV pOEMPDEV = NULL;


	// allocate memory for pOEMPDEV
    if(!(pOEMPDEV = (POEMPDEV)GlobalAllocPtr(GHND, sizeof(OEMPDEV))))
	{
		WARNING(UNITEXT("OEMEnablePDEV(), error(%d) occured while allocating memory for pOEMPDEV!\r\n"), GetLastError());
        return NULL;
	}


	DWORD  InnerLoop = 0;
    DWORD  DDIIndex = 0;	// HOOK function's index
    PDRVFN pdrvfn = NULL;	// pointer to DRVFN structure in DRVENABLEDATA

    // fill in OEMPDEV with Unidrv's DDI function table
    for(short Loop = 0; Loop < MAX_DDI_HOOKS; Loop++)
    {
        // search through Unidrv's hooks and locate the function ptr
        DDIIndex = OEMHookFuncs[Loop].iFunc;

        for(InnerLoop = pded->c, pdrvfn = pded->pdrvfn; InnerLoop > 0; InnerLoop--, pdrvfn++)
        {
            if(DDIIndex == pdrvfn->iFunc)
            {
                pOEMPDEV->pfnUnidrv[Loop] = pdrvfn->pfn;
                break;
            }
        }

        if(InnerLoop == 0)
        {
            // didn't find the Unidrv hook. Should happen only with DrvRealizeBrush
            pOEMPDEV->pfnUnidrv[Loop] = NULL;
        }
    }

	// Unidrv helper pointer
	pOEMPDEV->pOEMHelp = pOEMHelp;

	// create heap
	pOEMPDEV->hHeap = HeapCreate(0, 0, 0);
	if(pOEMPDEV->hHeap == NULL)
	{
		WARNING(UNITEXT("OEMEnablePDEV(), error(%d) occured while creating heap, clean and return error.\r\n"), GetLastError());

		// free memory
		GlobalFreePtr(pOEMPDEV);
		return NULL;
	}

	return pOEMPDEV;
}

/*
 * Function: OEMDisablePDEV
 *
 * Purpose: Delete oem PDEV structure that was allocated by OEMEnablePDEV method.
 *
 * Action: Destroy heap and free memory.
 *
 * Return: NULL
 */
VOID APIENTRY OEMDisablePDEV(
    PDEVOBJ pdevobj)
{
	// Free memory for OEMPDEV and any memory block that hangs off OEMPDEV
	if(pdevobj->pdevOEM)
	{
		// destroy heap
		HeapDestroy(((POEMPDEV)pdevobj->pdevOEM)->hHeap);

		// free OEMPDEV memory
		GlobalFreePtr(pdevobj->pdevOEM);
	}
}

/*
 * Function: OEMResetPDEV
 *
 * Purpose: Reset oem PDEV structure.
 *
 * Return: BOOL
 */
BOOL APIENTRY OEMResetPDEV(
    PDEVOBJ pdevobjOld,
    PDEVOBJ pdevobjNew)
{
    // if you want to carry over anything from old pdev to new pdev, do it here

    return TRUE;
}
