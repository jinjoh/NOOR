/**
 * Manager for random events
 *
 * $Id: SCA_RandomEventManager.cpp 23490 2009-09-25 16:30:15Z campbellbarton $
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
#include "SCA_RandomEventManager.h"
#include "SCA_LogicManager.h"
#include "SCA_ISensor.h"
#include <vector>
using namespace std;

#include <iostream>
#include <stdio.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

SCA_RandomEventManager::SCA_RandomEventManager(class SCA_LogicManager* logicmgr)
		: SCA_EventManager(logicmgr, RANDOM_EVENTMGR)
{
}


void SCA_RandomEventManager::NextFrame()
{
	SG_DList::iterator<SCA_ISensor> it(m_sensors);
	for (it.begin();!it.end();++it)
	{
		(*it)->Activate(m_logicmgr);
	}
}

