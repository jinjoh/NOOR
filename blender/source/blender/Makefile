#
# $Id: Makefile 23463 2009-09-24 21:22:24Z ben2610 $
#
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
# The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
# All rights reserved.
#
# The Original Code is: all of this file.
#
# Contributor(s): none yet.
#
# ***** END GPL LICENSE BLOCK *****
#
#

include nan_definitions.mk

DIRS = windowmanager editors blenloader readblenfile
DIRS += avi imbuf render blenlib blenkernel blenpluginapi
DIRS += makesdna makesrna
DIRS += python nodes gpu
DIRS += blenfont ikplugin

ifeq ($(WITH_QUICKTIME), true)
	DIRS += quicktime
endif

ifeq ($(WITH_OPENJPEG), true)
    CFLAGS += -DWITH_OPENJPEG -I../../../../extern/libopenjpeg
endif


DIR = $(OCGDIR)/blender
SOURCEDIR = source/blender
TESTDIRS = deflate streamglue

include nan_subdirs.mk
