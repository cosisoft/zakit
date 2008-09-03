/*
 * @file uni/ddifont.h
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

#ifndef _DDIFONT_H
#define _DDIFONT_H


/*
 *  Prototypes
 */

/*
ULONG APIENTRY
OEMFontManagement(
    SURFOBJ    *pso,
    FONTOBJ    *pfo,
    ULONG       iMode,
    ULONG       cjIn,
    PVOID       pvIn,
    ULONG       cjOut,
    PVOID       pvOut
);
*/

/*
ULONG APIENTRY
OEMGetGlyphMode(
    DHPDEV      dhpdev,
    FONTOBJ    *pfo
);
*/

/*
BOOL APIENTRY
OEMQueryAdvanceWidths(
    DHPDEV      dhpdev,
    FONTOBJ    *pfo,
    ULONG       iMode,
    HGLYPH     *phg,
    PVOID       pvWidths,
    ULONG       cGlyphs
);
*/

/*
PIFIMETRICS APIENTRY
OEMQueryFont(
    DHPDEV      dhpdev,
    ULONG       iFile,
    ULONG       iFace,
    ULONG      *pid
);
*/

/*
LONG APIENTRY
OEMQueryFontData(
    DHPDEV      dhpdev,
    FONTOBJ    *pfo,
    ULONG       iMode,
    HGLYPH      hg,
    GLYPHDATA  *pgd,
    PVOID       pv,
    ULONG       cjSize
);
*/

/*
PVOID APIENTRY
OEMQueryFontTree(
    DHPDEV      dhpdev,
    ULONG       iFile,
    ULONG       iFace,
    ULONG       iMode,
    ULONG      *pid
);
*/


#endif // !_DDIFONT_H
