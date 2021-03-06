/**
 * $Id: iksolver_plugin.h 23467 2009-09-25 01:13:07Z gsrb3d $
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
 * Original author: Benoit Bolsee
 * Contributor(s): 
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#ifndef IKSOLVER_PLUGIN_H
#define IKSOLVER_PLUGIN_H

#include "ikplugin_api.h"

#ifdef __cplusplus
extern "C" {
#endif

void iksolver_initialize_tree(struct Scene *scene, struct Object *ob, float ctime);
void iksolver_execute_tree(struct Scene *scene, struct Object *ob,  struct bPoseChannel *pchan, float ctime);

#ifdef __cplusplus
}
#endif

#endif // IKSOLVER_PLUGIN_H

