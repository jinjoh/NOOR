/**
 * blenlib/BLI_scanfill.h    mar 2001 Nzc
 *
 * Filling meshes.
 *
 * $Id: BLI_scanfill.h 17965 2008-12-20 10:02:00Z elubie $
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

#ifndef BLI_SCANFILL_H
#define BLI_SCANFILL_H

/**
 * @attention Defined in scanfill.c
 */
extern struct ListBase fillvertbase;
extern struct ListBase filledgebase;
extern struct ListBase fillfacebase;

struct EditVert;

#ifdef __cplusplus
extern "C" {
#endif

/* scanfill.c: used in displist only... */
struct EditVert *BLI_addfillvert(float *vec);
struct EditEdge *BLI_addfilledge(struct EditVert *v1, struct EditVert *v2);
int BLI_edgefill(int mode, int mat_nr);
void BLI_end_edgefill(void);

/* These callbacks are needed to make the lib finction properly */

/**
 * Set a function taking a char* as argument to flag errors. If the
 * callback is not set, the error is discarded.
 * @param f The function to use as callback
 * @attention used in creator.c
 */
void BLI_setErrorCallBack(void (*f)(char*));

/**
 * Set a function to be able to interrupt the execution of processing
 * in this module. If the function returns true, the execution will
 * terminate gracefully. If the callback is not set, interruption is
 * not possible.
 * @param f The function to use as callback
 * @attention used in creator.c
 */
void BLI_setInterruptCallBack(int (*f)(void));

#ifdef __cplusplus
}
#endif

#endif

