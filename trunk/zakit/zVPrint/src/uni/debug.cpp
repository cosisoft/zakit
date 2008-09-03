/*
 * @file lib/debug.cpp Debug functions.
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


/*
 *  Internal defines
 */

#define DEBUG_BUFFER_SIZE       1024

#define TERSE_MSG

// determine what level of debugging messages to eject
#ifdef VERBOSE_MSG
    #define DEBUG_LEVEL     DBG_VERBOSE
#elif defined TERSE_MSG
    #define DEBUG_LEVEL     DBG_TERSE
#elif defined WARNING_MSG
    #define DEBUG_LEVEL     DBG_WARNING
#elif defined ERROR_MSG
    #define DEBUG_LEVEL     DBG_ERROR
#elif defined NO_DBG_MSG
    #define DEBUG_LEVEL     DBG_NONE
#else
    #define DEBUG_LEVEL     DBG_WARNING
#endif


/*
 *  Globals
 */

int giDebugLevel = DEBUG_LEVEL;


/*
 *  Internal prototypes
 */

static BOOL DebugMessageV(LPCSTR lpszMessage, va_list arglist);
static BOOL DebugMessageV(DWORD dwSize, LPCWSTR lpszMessage, va_list arglist);


/*
 *  Internal implementations
 */

/*
 * Function: DebugMessageV(for ANSI)
 *
 * Purpose: Outputs variable argument debug string.
 *
 * Return: BOOL
 */
static BOOL DebugMessageV(
	LPCSTR  lpszMessage,
	va_list arglist)
{
	DWORD   dwSize = DEBUG_BUFFER_SIZE;
	LPSTR   lpszMsgBuf = NULL;

	// parameter checking
	if((NULL == lpszMessage) || (0 == dwSize))
		return FALSE;

	do
	{
		// allocate memory for message buffer
		if(NULL != lpszMsgBuf)
		{
			delete lpszMsgBuf;
			dwSize *= 2;
		}
		lpszMsgBuf = new CHAR[dwSize + 1];
		if(NULL == lpszMsgBuf)
			return FALSE;

	// pass the variable parameters to wvsprintf to be formated
	} while (_vsnprintf(lpszMsgBuf, dwSize, lpszMessage, arglist) < 0);

	// dump string to Debug output
	OutputDebugStringA(lpszMsgBuf);

	// cleanup
	delete lpszMsgBuf;

	return TRUE;
}

/*
 * Function: DebugMessageV(for Unicode)
 *
 * Purpose: Outputs variable argument debug string.
 *
 * Return: BOOL
 */
static BOOL DebugMessageV(
	DWORD   dwSize,
	LPCWSTR lpszMessage,
	va_list arglist)
{
    LPWSTR     lpszMsgBuf;

    // parameter checking
    if((NULL == lpszMessage) || (0 == dwSize))
		return FALSE;

    // allocate memory for message buffer
    lpszMsgBuf = new WCHAR[dwSize + 1];
    if(NULL == lpszMsgBuf) // error handling
        return FALSE;

    // pass the variable parameters to wvsprintf to be formated
    vswprintf(lpszMsgBuf, lpszMessage, arglist);

    // dump string to debug output
    OutputDebugStringW(lpszMsgBuf);

    // clean up
    delete lpszMsgBuf;

    return TRUE;
}


/*
 *  Implementations
 */

/*
 * Function: DebugMessage(for ANSI)
 *
 * Purpose: Outputs variable argument debug string.
 *
 * Return: BOOL
 */
BOOL DebugMessage(
	LPCSTR lpszMessage,
	...)
{
    BOOL    bResult;
    va_list VAList;

    // pass the variable parameters to DebugMessageV for processing
    va_start(VAList, lpszMessage);
    bResult = DebugMessageV(lpszMessage, VAList);
    va_end(VAList);

    return bResult;
}

/*
 * Function: DebugMessage(for Unicode)
 *
 * Purpose: Outputs variable argument debug string.
 *
 * Return: BOOL
 */
BOOL DebugMessage(
	LPCWSTR lpszMessage,
	...)
{
    BOOL    bResult;
    va_list VAList;

    // pass the variable parameters to DebugMessageV to be processed
    va_start(VAList, lpszMessage);
    bResult = DebugMessageV(MAX_PATH, lpszMessage, VAList);
    va_end(VAList);

    return bResult;
}
