/*
 * $Id: buildinfo.c 23991 2009-10-20 08:01:17Z blendix $
 *
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define STRINGIFY(x) XSTRINGIFY(x)
#define XSTRINGIFY(x) #x

#ifdef BUILD_DATE
const char * build_date=STRINGIFY(BUILD_DATE);
const char * build_time=STRINGIFY(BUILD_TIME);
const char * build_rev=STRINGIFY(BUILD_REV);
const char * build_platform=STRINGIFY(BUILD_PLATFORM);
const char * build_type=STRINGIFY(BUILD_TYPE);
#endif