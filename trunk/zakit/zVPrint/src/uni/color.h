/*
 * @file uni/color.h
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

#ifndef _COLOR_H
#define _COLOR_H


/*
 *  Defines
 */

// color value (RGB)
#define VALUE_BLACK			0x00000000
#define VALUE_RED			0x000000ff
#define VALUE_GREEN			0x0000ff00
#define VALUE_BLUE			0x00ff0000
#define VALUE_YELLOW		0x0000ffff
#define VALUE_MAGENTA		0x00ff00ff
#define VALUE_CYAN			0x00ffff00


/*
 *  Prototypes
 */

short differ_color(DWORD Color);


#endif // !_COLOR_H
