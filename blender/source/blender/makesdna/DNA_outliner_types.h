/**
 * $Id: DNA_outliner_types.h 19419 2009-03-26 14:05:33Z blendix $ 
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
#ifndef DNA_OUTLINER_TYPES_H
#define DNA_OUTLINER_TYPES_H

#include "DNA_listBase.h"

struct ID;

typedef struct TreeStoreElem {
	short type, nr, flag, used;
	struct ID *id;
} TreeStoreElem;

typedef struct TreeStore {
	int totelem, usedelem;
	TreeStoreElem *data;
} TreeStore;

/* TreeStoreElem->flag */
#define TSE_CLOSED		1
#define TSE_SELECTED	2
#define TSE_TEXTBUT		4

/* TreeStoreElem types in BIF_outliner.h */

#endif

