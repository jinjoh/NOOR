/* $Id: BKE_subsurf.h 18374 2009-01-06 18:59:03Z nicholasbishop $ 
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
#ifndef BKE_SUBSURF_H
#define BKE_SUBSURF_H

struct Mesh;
struct Object;
struct DerivedMesh;
struct EditMesh;
struct MultiresSubsurf;
struct SubsurfModifierData;

struct DerivedMesh *subsurf_make_derived_from_derived(
                        struct DerivedMesh *dm,
                        struct SubsurfModifierData *smd,
                        int useRenderParams, float (*vertCos)[3],
                        int isFinalCalc, int editMode);

struct DerivedMesh *subsurf_make_derived_from_derived_with_multires(
                        struct DerivedMesh *dm,
                        struct SubsurfModifierData *smd,
			struct MultiresSubsurf *ms,
                        int useRenderParams, float (*vertCos)[3],
                        int isFinalCalc, int editMode);

void subsurf_calculate_limit_positions(Mesh *me, float (*positions_r)[3]);

#endif
