/*
 * @file lib/debug.h
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

#ifndef _DEBUG_H
#define _DEBUG_H


/*
 *  Defines
 */

/*
 * These macros are used for debugging purposes. They expand
 * to white spaces on a free build. Here is a brief description
 * of what they do and how they are used:
 *
 * giDebugLevel
 *  Global variable which set the current debug level to control
 *  the amount of debug messages emitted.
 *
 * VERBOSE(msg)
 *  Display a message if the current debug level is <= DBG_VERBOSE.
 *
 * TERSE(msg)
 *  Display a message if the current debug level is <= DBG_TERSE.
 *
 * WARNING(msg)
 *  Display a message if the current debug level is <= DBG_WARNING.
 *
 * ERR(msg)
 *  Similiar to WARNING macro above - displays a message
 *  if the current debug level is <= DBG_ERROR.
 *
 * Usage:
 *  These macros require extra parantheses for the msg argument
 *  example, WARNING(("App passed NULL pointer, ignoring...\n"));
 */

#define DBG_VERBOSE 1
#define DBG_TERSE   2
#define DBG_WARNING 3
#define DBG_ERROR   4
#define DBG_NONE    5


#ifdef _DEBUG

	#define DebugMsg    DebugMessage

    extern int giDebugLevel;


    #define VERBOSE         if(giDebugLevel <= DBG_VERBOSE) DebugMsg
    #define TERSE           if(giDebugLevel <= DBG_TERSE) DebugMsg
    #define WARNING         if(giDebugLevel <= DBG_WARNING) DebugMsg
    #define ERR             if(giDebugLevel <= DBG_ERROR) DebugMsg

	#define UITEXT(s)         TEXT("UI:  ")  TEXT(s)
	#define UIERROR(s)        TEXT("ERROR ") UITEXT(s)

	#define UNITEXT(s)        TEXT("UNI:  ") TEXT(s)
	#define UNIERROR(s)       TEXT("ERROR ") UNITEXT(s)

	#define ERRORTEXT(s)      TEXT("ERROR: ") TEXT(s)

#else

    #define DebugMsg    NOP_FUNCTION

    #define VERBOSE     NOP_FUNCTION
    #define TERSE       NOP_FUNCTION
    #define WARNING     NOP_FUNCTION
    #define ERR         NOP_FUNCTION

	#define UITEXT(s)		NOP_FUNCTION
	#define UIERROR(s)		NOP_FUNCTION

	#define UNITEXT(s)		NOP_FUNCTION
	#define UNIERROR(s)		NOP_FUNCTION

	#define ERRORTEXT(s)	NOP_FUNCTION

#endif


/*
 *  Prototypes
 */

BOOL DebugMessage(LPCSTR, ...);
BOOL DebugMessage(LPCWSTR, ...);


#endif // !_DEBUG_H
