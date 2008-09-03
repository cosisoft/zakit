/*
 * @file uni/color.cpp
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
#include "devmode.h"
#include "color.h"


/*
 *  Implementations
 */

/*
 * Function: differ_color
 *
 * Purpose: Judge color.
 *
 * Return Value: color index
 */
short differ_color(DWORD Color)
{
	short Index = PCOLOR_BLACK;


	switch(Color)
	{
	case VALUE_RED:
		Index = PCOLOR_RED;
		break;

	case VALUE_GREEN:
		Index = PCOLOR_GREEN;
		break;

	case VALUE_BLUE:
		Index = PCOLOR_BLUE;
		break;

	case VALUE_YELLOW:
		Index = PCOLOR_YELLOW;
		break;

	case VALUE_MAGENTA:
		Index = PCOLOR_MAGENTA;
		break;

	case VALUE_CYAN:
		Index = PCOLOR_CYAN;
		break;
	}

	return Index;
}
