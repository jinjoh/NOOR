/**
 * $Id: GlutKeyboardManager.h 13161 2008-01-07 19:13:47Z hos $
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

#ifndef NAN_INCLUDED_GlutKeyboardManager
#define NAN_INCLUDED_GlutKeyboardManager

#include "MEM_NonCopyable.h"
#include "MEM_SmartPtr.h"
#include "GHOST_Types.h"

// inherit from GlutKeyboardHandler and installl the drawer in the singleton
// class GlutKeyboardManager.

class GlutKeyboardHandler : public MEM_NonCopyable {
public :

	virtual 
		void
	HandleKeyboard(
		GHOST_TKey key,
		int x,
		int y
	)= 0;

	virtual 
	~GlutKeyboardHandler(
	){};		
};

class GlutKeyboardManager : public MEM_NonCopyable{

public :

	static
		GlutKeyboardManager *
	Instance(
	);

	static
		void
	HandleKeyboard(
		GHOST_TKey key,
		int x,
		int y
	);

		void
	InstallHandler(
		GlutKeyboardHandler *
	);

		void
	ReleaseHandler(
	);

	~GlutKeyboardManager(
	);

private :

	GlutKeyboardManager (
	) :
		m_handler (0)
	{
	};
	
	GlutKeyboardHandler * m_handler;

	static MEM_SmartPtr<GlutKeyboardManager> m_s_instance;
};	


#endif
