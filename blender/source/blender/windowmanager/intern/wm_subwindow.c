/**
 * $Id: wm_subwindow.c 23615 2009-10-03 15:35:01Z imbusy $
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
 * Contributor(s): 2007 Blender Foundation (refactor)
 *
 * ***** END GPL LICENSE BLOCK *****
 *
 *
 * Subwindow opengl handling. 
 * BTW: subwindows open/close in X11 are way too slow, tried it, and choose for my own system... (ton)
 * 
 */

#include <string.h>

#include "MEM_guardedalloc.h"

#include "DNA_windowmanager_types.h"
#include "DNA_screen_types.h"

#include "BLI_blenlib.h"
#include "BLI_arithb.h"

#include "BKE_context.h"
#include "BKE_global.h"

#include "BIF_gl.h"
#include "BIF_glutil.h"

#include "WM_api.h"
#include "wm_subwindow.h"
#include "wm_window.h"

/* wmSubWindow stored in wmWindow... but not exposed outside this C file */
/* it seems a bit redundant (area regions can store it too, but we keep it
   because we can store all kind of future opengl fanciness here */

/* we use indices and array because:
   - index has safety, no pointers from this C file hanging around
   - fast lookups of indices with array, list would give overhead
   - old code used it this way...
   - keep option open to have 2 screens using same window
*/

typedef struct wmSubWindow {
	struct wmSubWindow *next, *prev;
	
	rcti winrct;
	int swinid;
	
	float viewmat[4][4], winmat[4][4];
	float viewmat1[4][4], winmat1[4][4];
} wmSubWindow;


/* ******************* open, free, set, get data ******************** */

/* not subwindow itself */
static void wm_subwindow_free(wmSubWindow *swin)
{
	/* future fancy stuff */
}

void wm_subwindows_free(wmWindow *win)
{
	wmSubWindow *swin;
	
	for(swin= win->subwindows.first; swin; swin= swin->next)
		wm_subwindow_free(swin);
	
	BLI_freelistN(&win->subwindows);
}


int wm_subwindow_get(wmWindow *win)	
{
	if(win->curswin)
		return win->curswin->swinid;
	return 0;
}

static wmSubWindow *swin_from_swinid(wmWindow *win, int swinid)
{
	wmSubWindow *swin;
	
	for(swin= win->subwindows.first; swin; swin= swin->next)
		if(swin->swinid==swinid)
			break;
	return swin;
}

void wm_subwindow_getsize(wmWindow *win, int swinid, int *x, int *y) 
{
	wmSubWindow *swin= swin_from_swinid(win, swinid);

	if(swin) {
		*x= swin->winrct.xmax - swin->winrct.xmin + 1;
		*y= swin->winrct.ymax - swin->winrct.ymin + 1;
	}
}

void wm_subwindow_getorigin(wmWindow *win, int swinid, int *x, int *y)
{
	wmSubWindow *swin= swin_from_swinid(win, swinid);

	if(swin) {
		*x= swin->winrct.xmin;
		*y= swin->winrct.ymin;
	}
}

void wm_subwindow_getmatrix(wmWindow *win, int swinid, float mat[][4])
{
	wmSubWindow *swin= swin_from_swinid(win, swinid);

	if(swin)
		Mat4MulMat4(mat, swin->viewmat, swin->winmat);
}

/* always sets pixel-precise 2D window/view matrices */
/* coords is in whole pixels. xmin = 15, xmax= 16: means window is 2 pix big */
int wm_subwindow_open(wmWindow *win, rcti *winrct)
{
	wmSubWindow *swin;
	int width, height;
	int freewinid= 1;
	
	for(swin= win->subwindows.first; swin; swin= swin->next)
		if(freewinid <= swin->swinid)
			freewinid= swin->swinid+1;

	win->curswin= swin= MEM_callocN(sizeof(wmSubWindow), "swinopen");
	BLI_addtail(&win->subwindows, swin);
	
	if(G.f & G_DEBUG) printf("swin %d added\n", freewinid);
	swin->swinid= freewinid;
	swin->winrct= *winrct;

	Mat4One(swin->viewmat);
	Mat4One(swin->winmat);
	
	/* and we appy it all right away */
	wmSubWindowSet(win, swin->swinid);
	
	/* extra service */
	wm_subwindow_getsize(win, swin->swinid, &width, &height);
	wmOrtho2(-0.375, (float)width-0.375, -0.375, (float)height-0.375);
	wmLoadIdentity();

	return swin->swinid;
}


void wm_subwindow_close(wmWindow *win, int swinid)
{
	wmSubWindow *swin= swin_from_swinid(win, swinid);

	if (swin) {
		if (swin==win->curswin)
			win->curswin= NULL;
		wm_subwindow_free(swin);
		BLI_remlink(&win->subwindows, swin);
		MEM_freeN(swin);
	} 
	else {
		printf("wm_subwindow_close: Internal error, bad winid: %d\n", swinid);
	}

}

/* pixels go from 0-99 for a 100 pixel window */
void wm_subwindow_position(wmWindow *win, int swinid, rcti *winrct)
{
	wmSubWindow *swin= swin_from_swinid(win, swinid);
	
	if(swin) {
		int width, height;
		
		swin->winrct= *winrct;
		
		/* CRITICAL, this clamping ensures that
			* the viewport never goes outside the screen
			* edges (assuming the x, y coords aren't
					 * outside). This caused a hardware lock
			* on Matrox cards if it happens.
			* 
			* Really Blender should never _ever_ try
			* to do such a thing, but just to be safe
			* clamp it anyway (or fix the bScreen
		    * scaling routine, and be damn sure you
		    * fixed it). - zr  (2001!)
			*/
		
		if (swin->winrct.xmax > win->sizex)
			swin->winrct.xmax= win->sizex;
		if (swin->winrct.ymax > win->sizey)
			swin->winrct.ymax= win->sizey;
		
		/* extra service */
		wmSubWindowSet(win, swinid);
		wm_subwindow_getsize(win, swinid, &width, &height);
		wmOrtho2(-0.375, (float)width-0.375, -0.375, (float)height-0.375);
	}
	else {
		printf("wm_subwindow_position: Internal error, bad winid: %d\n", swinid);
	}
}

/* ---------------- WM versions of OpenGL calls, using glBlah() syntax ------------------------ */
/* ----------------- exported in WM_api.h ------------------------------------------------------ */

/* internal state, no threaded opengl! XXX */
static wmWindow *_curwindow= NULL;
static wmSubWindow *_curswin= NULL;

void wmSubWindowScissorSet(wmWindow *win, int swinid, rcti *srct)
{
	int width, height;
	_curswin= swin_from_swinid(win, swinid);
	
	if(_curswin==NULL) {
		printf("wmSubWindowSet %d: doesn't exist\n", swinid);
		return;
	}
	
	win->curswin= _curswin;
	_curwindow= win;
	
	width= _curswin->winrct.xmax - _curswin->winrct.xmin + 1;
	height= _curswin->winrct.ymax - _curswin->winrct.ymin + 1;
	glViewport(_curswin->winrct.xmin, _curswin->winrct.ymin, width, height);

	if(srct) {
		width= srct->xmax - srct->xmin + 1;
		height= srct->ymax - srct->ymin + 1;
		glScissor(srct->xmin, srct->ymin, width, height);
	}
	else
		glScissor(_curswin->winrct.xmin, _curswin->winrct.ymin, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&_curswin->winmat[0][0]);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&_curswin->viewmat[0][0]);
	
	glFlush();
}


/* enable the WM versions of opengl calls */
void wmSubWindowSet(wmWindow *win, int swinid)
{
	wmSubWindowScissorSet(win, swinid, NULL);
}

void wmLoadMatrix(float mat[][4])
{
	if(_curswin==NULL) return;
	
	glLoadMatrixf(mat);
	
	if (glaGetOneInteger(GL_MATRIX_MODE)==GL_MODELVIEW)
		Mat4CpyMat4(_curswin->viewmat, mat);
	else
		Mat4CpyMat4(_curswin->winmat, mat);
}

void wmGetMatrix(float mat[][4])
{
	if(_curswin==NULL) return;
	
	if (glaGetOneInteger(GL_MATRIX_MODE)==GL_MODELVIEW) {
		Mat4CpyMat4(mat, _curswin->viewmat);
	} else {
		Mat4CpyMat4(mat, _curswin->winmat);
	}
}

void wmMultMatrix(float mat[][4])
{
	if(_curswin==NULL) return;
	
	glMultMatrixf((float*) mat);
	
	if (glaGetOneInteger(GL_MATRIX_MODE)==GL_MODELVIEW)
		glGetFloatv(GL_MODELVIEW_MATRIX, (float *)_curswin->viewmat);
	else
		glGetFloatv(GL_MODELVIEW_MATRIX, (float *)_curswin->winmat);
}

static int debugpush= 0;

void wmPushMatrix(void)
{
	if(_curswin==NULL) return;
	
	if(debugpush)
		printf("wmPushMatrix error already pushed\n");
	debugpush= 1;
	
	Mat4CpyMat4(_curswin->viewmat1, _curswin->viewmat);
	Mat4CpyMat4(_curswin->winmat1, _curswin->winmat);
}

void wmPopMatrix(void)
{
	if(_curswin==NULL) return;
	
	if(debugpush==0)
		printf("wmPopMatrix error nothing popped\n");
	debugpush= 0;
	
	Mat4CpyMat4(_curswin->viewmat, _curswin->viewmat1);
	Mat4CpyMat4(_curswin->winmat, _curswin->winmat1);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&_curswin->winmat[0][0]);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&_curswin->viewmat[0][0]);
	
}

void wmGetSingleMatrix(float mat[][4])
{
	if(_curswin)
		Mat4MulMat4(mat, _curswin->viewmat, _curswin->winmat);
}

void wmScale(float x, float y, float z)
{
	if(_curswin==NULL) return;
	
	glScalef(x, y, z);
	
	if (glaGetOneInteger(GL_MATRIX_MODE)==GL_MODELVIEW)
		glGetFloatv(GL_MODELVIEW_MATRIX, (float *)_curswin->viewmat);
	else
		glGetFloatv(GL_MODELVIEW_MATRIX, (float *)_curswin->winmat);
	
}

void wmLoadIdentity(void)
{
	if(_curswin==NULL) return;
	
	if (glaGetOneInteger(GL_MATRIX_MODE)==GL_MODELVIEW)
		Mat4One(_curswin->viewmat);
	else
		Mat4One(_curswin->winmat);
	
	glLoadIdentity();
}

void wmFrustum(float x1, float x2, float y1, float y2, float n, float f)
{
	if(_curswin) {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(x1, x2, y1, y2, n, f);

		glGetFloatv(GL_PROJECTION_MATRIX, (float *)_curswin->winmat);
		glMatrixMode(GL_MODELVIEW);
	}
}

void wmOrtho(float x1, float x2, float y1, float y2, float n, float f)
{
	if(_curswin) {
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		glOrtho(x1, x2, y1, y2, n, f);
		
		glGetFloatv(GL_PROJECTION_MATRIX, (float *)_curswin->winmat);
		glMatrixMode(GL_MODELVIEW);
	}
}

void wmOrtho2(float x1, float x2, float y1, float y2)
{
	/* prevent opengl from generating errors */
	if(x1==x2) x2+=1.0;
	if(y1==y2) y2+=1.0;
	wmOrtho(x1, x2, y1, y2, -100, 100);
}


/* *************************** Framebuffer color depth, for selection codes ********************** */

static int wm_get_colordepth(void)
{
	static int mainwin_color_depth= 0;	
	
	if(mainwin_color_depth==0) {
		GLint r, g, b;
		
		glGetIntegerv(GL_RED_BITS, &r);
		glGetIntegerv(GL_GREEN_BITS, &g);
		glGetIntegerv(GL_BLUE_BITS, &b);
		
		mainwin_color_depth= r + g + b;
		if(G.f & G_DEBUG) {
			printf("Color depth r %d g %d b %d\n", (int)r, (int)g, (int)b);
			glGetIntegerv(GL_AUX_BUFFERS, &r);
			printf("Aux buffers: %d\n", (int)r);
		}
	}
	return mainwin_color_depth;
}


#ifdef __APPLE__

/* apple seems to round colors to below and up on some configs */

unsigned int index_to_framebuffer(int index)
{
	unsigned int i= index;

	switch(wm_get_colordepth()) {
	case 12:
		i= ((i & 0xF00)<<12) + ((i & 0xF0)<<8) + ((i & 0xF)<<4);
		/* sometimes dithering subtracts! */
		i |= 0x070707;
		break;
	case 15:
	case 16:
		i= ((i & 0x7C00)<<9) + ((i & 0x3E0)<<6) + ((i & 0x1F)<<3);
		i |= 0x030303;
		break;
	case 24:
		break;
	default:	// 18 bits... 
		i= ((i & 0x3F000)<<6) + ((i & 0xFC0)<<4) + ((i & 0x3F)<<2);
		i |= 0x010101;
		break;
	}
	
	return i;
}

#else

/* this is the old method as being in use for ages.... seems to work? colors are rounded to lower values */

unsigned int index_to_framebuffer(int index)
{
	unsigned int i= index;
	
	switch(wm_get_colordepth()) {
		case 8:
			i= ((i & 48)<<18) + ((i & 12)<<12) + ((i & 3)<<6);
			i |= 0x3F3F3F;
			break;
		case 12:
			i= ((i & 0xF00)<<12) + ((i & 0xF0)<<8) + ((i & 0xF)<<4);
			/* sometimes dithering subtracts! */
			i |= 0x0F0F0F;
			break;
		case 15:
		case 16:
			i= ((i & 0x7C00)<<9) + ((i & 0x3E0)<<6) + ((i & 0x1F)<<3);
			i |= 0x070707;
			break;
		case 24:
			break;
		default:	// 18 bits... 
			i= ((i & 0x3F000)<<6) + ((i & 0xFC0)<<4) + ((i & 0x3F)<<2);
			i |= 0x030303;
			break;
	}
	
	return i;
}

#endif

void WM_set_framebuffer_index_color(int index)
{
	cpack(index_to_framebuffer(index));
}

int WM_framebuffer_to_index(unsigned int col)
{
	if (col==0) return 0;

	switch(wm_get_colordepth()) {
	case 8:
		return ((col & 0xC00000)>>18) + ((col & 0xC000)>>12) + ((col & 0xC0)>>6);
	case 12:
		return ((col & 0xF00000)>>12) + ((col & 0xF000)>>8) + ((col & 0xF0)>>4);
	case 15:
	case 16:
		return ((col & 0xF80000)>>9) + ((col & 0xF800)>>6) + ((col & 0xF8)>>3);
	case 24:
		return col & 0xFFFFFF;
	default: // 18 bits...
		return ((col & 0xFC0000)>>6) + ((col & 0xFC00)>>4) + ((col & 0xFC)>>2);
	}		
}


/* ********** END MY WINDOW ************** */

#if 0 // XXX not used...
#ifdef WIN32
static int is_a_really_crappy_nvidia_card(void) {
	static int well_is_it= -1;

		/* Do you understand the implication? Do you? */
	if (well_is_it==-1)
		well_is_it= (strcmp((char*) glGetString(GL_VENDOR), "NVIDIA Corporation") == 0);

	return well_is_it;
}
#endif
#endif // XXX not used...

