#!/usr/bin/env python

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
# Contributor(s): Jonathan Merritt.
#
# ***** END GPL LICENSE BLOCK *****

#
# This script generates a C source file and a header file that implement
# dynamic loading functionality for libtiff.
# If you need to make more functions from libtiff available, then simply add
# them to the tiff_functions[] list below.
#


FILENAME   = 'dynlibtiff'
C_FILENAME = '%s.c' % FILENAME
H_FILENAME = '%s.h' % FILENAME


COMMENT = \
"""/**
 * Dynamically loaded libtiff support.
 *
 * This file is automatically generated by the gen_dynlibtiff.py script.
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
 * Contributor(s): Jonathan Merritt.
 *
 * ***** END GPL LICENSE BLOCK *****
 */
 
/**
 * To use the dynamic libtiff support, you must initialize the library using:
 *	libtiff_init()
 * This attempts to load libtiff dynamically at runtime.  G.have_libtiff will
 * be set to indicate whether or not libtiff is available.  If libtiff is
 * not available, Blender can proceed with no ill effects, provided that
 * it does not attempt to use any of the libtiff_ functions.  When you're
 * finished, close the library with:
 *	libtiff_exit()
 * These functions are both declared in IMB_imbuf.h
 *
 * The functions provided by dyn_libtiff.h are the same as those in the
 * normal static / shared libtiff, except that they are prefixed by the 
 * string "libtiff_" to indicate that they belong to a dynamically-loaded 
 * version.
 */
"""


C_EXTRA = \
"""
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "BLI_blenlib.h"

#include "imbuf.h"
#include "imbuf_patch.h"
#include "IMB_imbuf.h"

#include "BKE_global.h"
#include "PIL_dynlib.h"

/*********************
 * LOCAL DEFINITIONS *
 *********************/
PILdynlib *libtiff = NULL;
void  libtiff_loadlibtiff(void);
void* libtiff_findsymbol(char*);
int   libtiff_load_symbols(void);


/**************************
 * LIBRARY INITIALIZATION *
 **************************/

void libtiff_loadlibtiff(void)
{
	char *filename;
	libtiff = NULL;

	filename = getenv("BF_TIFF_LIB");
	if (filename) libtiff = PIL_dynlib_open(filename);
	if (libtiff != NULL)  return;

	/* Try to find libtiff in a couple of standard places */
	libtiff = PIL_dynlib_open("libtiff.so");
	if (libtiff != NULL)  return;
	libtiff = PIL_dynlib_open("libtiff.so.3");
	if (libtiff != NULL)  return;
	libtiff = PIL_dynlib_open("libtiff.dll");
	if (libtiff != NULL)  return;
	libtiff = PIL_dynlib_open("/usr/lib/libtiff.so");
	if (libtiff != NULL)  return;
	libtiff = PIL_dynlib_open("/usr/lib/libtiff.so.3");
	if (libtiff != NULL)  return;
	/* OSX has version specific library */
#ifdef __x86_64__
	libtiff = PIL_dynlib_open("/usr/lib64/libtiff.so.3");
	if (libtiff != NULL)  return;
#endif
	libtiff = PIL_dynlib_open("/usr/local/lib/libtiff.so");
	if (libtiff != NULL)  return;
	/* For solaris */
	libtiff = PIL_dynlib_open("/usr/openwin/lib/libtiff.so");

}

void *libtiff_findsymbol(char *name)
{
	void *symbol = NULL;
	assert(libtiff != NULL);
	symbol = PIL_dynlib_find_symbol(libtiff, name);
	if (symbol == NULL) {
		printf("libtiff_findsymbol: error %s\\n",
			PIL_dynlib_get_error_as_string(libtiff));
		libtiff = NULL;
		G.have_libtiff = (0);
		return NULL;
	}
	return symbol;
}

void libtiff_init(void)
{
	if (libtiff != NULL) {
		printf("libtiff_init: Attempted to load libtiff twice!\\n");
		return;
	}
	libtiff_loadlibtiff();
	G.have_libtiff = ((libtiff != NULL) && (libtiff_load_symbols()));
}

void libtiff_exit(void)
{
	if (libtiff != NULL) {
		PIL_dynlib_close(libtiff);
		libtiff = NULL;
	}
}


"""


class CFun:
	def __init__(self, name, retType, args):
		self.name    = name
		self.retType = retType
		self.args    = args
	def getDynamicName(self):
		return ('libtiff_%s' % self.name)
	def getDynamicDecl(self):
		argstr = (('%s, '*len(self.args)) % tuple(self.args))[:-2]
		return ('%s (*%s)(%s)' % (self.retType, 
			self.getDynamicName(), argstr))
	def getLoadSymbol(self):
		dname = self.getDynamicName()
		return (
		"""\t/* Attempt to load %s */
	%s = libtiff_findsymbol("%s");
	if (%s == NULL) {
		return (0);
	}\n""" % (self.name, dname, self.name, dname))


# If you need more functions, add them to the list below, based upon entries
# in either tiffio.h or tiff.h.
tiff_functions = [
	CFun('TIFFClientOpen', 'TIFF*', ['const char*', 'const char*',
		'thandle_t', 'TIFFReadWriteProc', 'TIFFReadWriteProc',
		'TIFFSeekProc', 'TIFFCloseProc', 'TIFFSizeProc',
		'TIFFMapFileProc', 'TIFFUnmapFileProc']),
	CFun('TIFFClose', 'void', ['TIFF*']),
	CFun('TIFFGetField', 'int', ['TIFF*', 'ttag_t', '...']),
	CFun('TIFFOpen', 'TIFF*', ['const char*', 'const char*']),
	CFun('TIFFReadRGBAImage', 'int', ['TIFF*', 'uint32', 'uint32',
		'uint32*', 'int']),
	CFun('TIFFSetField', 'int', ['TIFF*', 'ttag_t', '...']),
	CFun('TIFFWriteEncodedStrip', 'tsize_t', ['TIFF*', 'tstrip_t',
		'tdata_t', 'tsize_t']),
	CFun('_TIFFfree', 'void', ['tdata_t']),
	CFun('_TIFFmalloc', 'tdata_t', ['tsize_t']),
]


def outputDynCFile(outfile, header_file_name):
	outfile.write(COMMENT)
	outfile.write('#include "%s"\n' % header_file_name)
	outfile.write(C_EXTRA)
	outfile.write('int libtiff_load_symbols(void)\n')
	outfile.write('{\n')
	for function in tiff_functions:
		outfile.write(function.getLoadSymbol())
	outfile.write('\treturn (1);\n')
	outfile.write('}\n')
	outfile.write("""
	
/*******************
 * SYMBOL POINTERS *
 *******************/\n\n""")
	for function in tiff_functions:
		outfile.write('%s = NULL;\n' % function.getDynamicDecl())


def outputDynHFile(outfile):
	outfile.write(COMMENT)
	outfile.write('#ifndef DYN_LIBTIFF_H\n')
	outfile.write('#include "tiffio.h"\n')
	for function in tiff_functions:
		outfile.write('extern %s;\n' % function.getDynamicDecl())
	outfile.write('#endif /* DYN_LIBTIFF_H */\n\n')


if __name__ == '__main__':
	outfile = file(C_FILENAME, 'w')
	outputDynCFile(outfile, H_FILENAME)
	outfile.close()
	outfile = file(H_FILENAME, 'w')
	outputDynHFile(outfile)
	outfile.close()
