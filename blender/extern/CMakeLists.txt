# $Id: CMakeLists.txt 23473 2009-09-25 02:40:10Z campbellbarton $
# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# The Original Code is Copyright (C) 2006, Blender Foundation
# All rights reserved.
#
# The Original Code is: all of this file.
#
# Contributor(s): Jacques Beaurain.
#
# ***** END GPL LICENSE BLOCK *****

IF(WITH_BULLET)
	ADD_SUBDIRECTORY(bullet2)
ENDIF(WITH_BULLET)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
	ADD_SUBDIRECTORY(binreloc)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")

ADD_SUBDIRECTORY(glew)

IF(WITH_OPENJPEG)
	ADD_SUBDIRECTORY(libopenjpeg)
ENDIF(WITH_OPENJPEG)

IF(WITH_LZO)
	ADD_SUBDIRECTORY(lzo)
ENDIF(WITH_LZO)

IF(WITH_LZMA)
	ADD_SUBDIRECTORY(lzma)
ENDIF(WITH_LZMA)
