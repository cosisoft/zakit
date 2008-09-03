/*
 * @file uni/dllentry.cpp
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


// need to export these functions as c declarations
extern "C" {

/*
 * Function: DllMain
 *
 * Action: DLL entry point.
 *
 * Return: BOOL
 */
BOOL WINAPI DllMain(HINSTANCE hInst, WORD wReason, LPVOID lpReserved)
{
	switch(wReason)
	{
	case DLL_PROCESS_ATTACH:
		VERBOSE(UNITEXT("Process attach.\r\n"));
		ghInstance = hInst;
		break;

	case DLL_THREAD_ATTACH:
		VERBOSE(UNITEXT("Thread attach.\r\n"));
		break;

	case DLL_PROCESS_DETACH:
		VERBOSE(UNITEXT("Process detach.\r\n"));
		break;

	case DLL_THREAD_DETACH:
		VERBOSE(UNITEXT("Thread detach.\r\n"));
		break;
	}

	return TRUE;
}

} // extern "C" closing bracket
