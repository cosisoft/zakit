/*
 * @file uni/ddicntl.h
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

#ifndef _DDICNTL_H
#define _DDICNTL_H


/*
 *  Prototypes
 */

BOOL APIENTRY
OEMStartDoc(
    SURFOBJ    *pso,
    PWSTR       pwszDocName,
    DWORD       dwJobId
);

BOOL APIENTRY
OEMStartPage(
    SURFOBJ    *pso
);

BOOL APIENTRY
OEMStartBanding(
    SURFOBJ	   *pso,
    POINTL     *pptl
);

BOOL APIENTRY
OEMNextBand(
    SURFOBJ    *pso,
    POINTL     *pptl
);

BOOL APIENTRY
OEMSendPage(
    SURFOBJ    *pso
);

BOOL APIENTRY
OEMEndDoc(
    SURFOBJ    *pso,
    FLONG       fl
);


#endif // !_DDICNTL_H
