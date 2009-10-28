/**
 * $Id: BL_SkinMeshObject.h 22603 2009-08-18 15:37:31Z campbellbarton $
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

#ifndef __BL_SKINMESHOBJECT
#define __BL_SKINMESHOBJECT

#ifdef WIN32
#pragma warning (disable:4786) // get rid of stupid stl-visual compiler debug warning
#endif //WIN32

#include "RAS_MeshObject.h"
#include "RAS_Deformer.h"
#include "RAS_IPolygonMaterial.h"

#include "BL_MeshDeformer.h"

class BL_SkinMeshObject : public RAS_MeshObject
{
protected:
	vector<int>				 m_cacheWeightIndex;

public:
	BL_SkinMeshObject(Mesh* mesh);
	~BL_SkinMeshObject();

	void UpdateBuckets(void* clientobj, double* oglmatrix,
		bool useObjectColor, const MT_Vector4& rgbavec, bool visible, bool culled);
	
	// for shape keys, 
	void CheckWeightCache(struct Object* obj);


#ifdef WITH_CXX_GUARDEDALLOC
public:
	void *operator new( unsigned int num_bytes) { return MEM_mallocN(num_bytes, "GE:BL_SkinMeshObject"); }
	void operator delete( void *mem ) { MEM_freeN(mem); }
#endif
};

#endif

