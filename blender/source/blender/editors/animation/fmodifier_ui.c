/**
 * $Id:
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
 * The Original Code is Copyright (C) 2009 Blender Foundation.
 * All rights reserved.
 *
 * 
 * Contributor(s): Blender Foundation, Joshua Leung
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/* User-Interface Stuff for F-Modifiers:
 * This file defines the (C-Coded) templates + editing callbacks needed 
 * by the interface stuff or F-Modifiers, as used by F-Curves in the Graph Editor,
 * and NLA-Strips in the NLA Editor.
 */
 
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "DNA_anim_types.h"
#include "DNA_action_types.h"
#include "DNA_object_types.h"
#include "DNA_space_types.h"
#include "DNA_scene_types.h"
#include "DNA_screen_types.h"
#include "DNA_userdef_types.h"

#include "MEM_guardedalloc.h"

#include "BLI_arithb.h"
#include "BLI_blenlib.h"
#include "BLI_editVert.h"
#include "BLI_rand.h"

#include "BKE_animsys.h"
#include "BKE_action.h"
#include "BKE_context.h"
#include "BKE_curve.h"
#include "BKE_customdata.h"
#include "BKE_depsgraph.h"
#include "BKE_fcurve.h"
#include "BKE_object.h"
#include "BKE_global.h"
#include "BKE_nla.h"
#include "BKE_scene.h"
#include "BKE_screen.h"
#include "BKE_utildefines.h"

#include "BIF_gl.h"

#include "WM_api.h"
#include "WM_types.h"

#include "RNA_access.h"
#include "RNA_define.h"

#include "ED_anim_api.h"
#include "ED_keyframing.h"
#include "ED_screen.h"
#include "ED_types.h"
#include "ED_util.h"

#include "UI_interface.h"
#include "UI_resources.h"
#include "UI_view2d.h"

// XXX! --------------------------------
/* temporary definition for limits of float number buttons (FLT_MAX tends to infinity with old system) */
#define UI_FLT_MAX 	10000.0f

/* ********************************************** */

#define B_REDR 					1
#define B_FMODIFIER_REDRAW		20

/* macro for use here to draw background box and set height */
// XXX for now, roundbox has it's callback func set to NULL to not intercept events
#define DRAW_BACKDROP(height) \
	{ \
		uiDefBut(block, ROUNDBOX, B_REDR, "", -3, yco-height, width+3, height-1, NULL, 5.0, 0.0, 12.0, (float)rb_col, ""); \
	}

/* callback to verify modifier data */
static void validate_fmodifier_cb (bContext *C, void *fcm_v, void *dummy)
{
	FModifier *fcm= (FModifier *)fcm_v;
	FModifierTypeInfo *fmi= fmodifier_get_typeinfo(fcm);
	
	/* call the verify callback on the modifier if applicable */
	if (fmi && fmi->verify_data)
		fmi->verify_data(fcm);
}

/* callback to set the active modifier */
static void activate_fmodifier_cb (bContext *C, void *fmods_v, void *fcm_v)
{
	ListBase *modifiers = (ListBase *)fmods_v;
	FModifier *fcm= (FModifier *)fcm_v;
	
	/* call API function to set the active modifier for active modifier-stack */
	set_active_fmodifier(modifiers, fcm);
}

/* callback to remove the given modifier  */
static void delete_fmodifier_cb (bContext *C, void *fmods_v, void *fcm_v)
{
	ListBase *modifiers = (ListBase *)fmods_v;
	FModifier *fcm= (FModifier *)fcm_v;
	
	/* remove the given F-Modifier from the active modifier-stack */
	remove_fmodifier(modifiers, fcm);
}

/* --------------- */
	
/* draw settings for generator modifier */
static void draw_modifier__generator(uiLayout *layout, ID *id, FModifier *fcm, short width)
{
	FMod_Generator *data= (FMod_Generator *)fcm->data;
	uiLayout *col, *row;
	uiBlock *block;
	uiBut *but;
	PointerRNA ptr;
	
	/* init the RNA-pointer */
	RNA_pointer_create(id, &RNA_FModifierFunctionGenerator, fcm, &ptr);
	
	/* basic settings (backdrop + mode selector + some padding) */
	col= uiLayoutColumn(layout, 1);
	block= uiLayoutGetBlock(layout);
	uiBlockBeginAlign(block);
		but= uiDefButR(block, MENU, B_FMODIFIER_REDRAW, NULL, 0, 0, width-30, UI_UNIT_Y, &ptr, "mode", -1, 0, 0, -1, -1, NULL);
		uiButSetFunc(but, validate_fmodifier_cb, fcm, NULL);
		
		uiDefButR(block, TOG, B_FMODIFIER_REDRAW, NULL, 0, 0, width-30, UI_UNIT_Y, &ptr, "additive", -1, 0, 0, -1, -1, NULL);
	uiBlockEndAlign(block);
	
	/* now add settings for individual modes */
	switch (data->mode) {
		case FCM_GENERATOR_POLYNOMIAL: /* polynomial expression */
		{
			float *cp = NULL;
			char xval[32];
			unsigned int i;
			
			/* draw polynomial order selector */
			row= uiLayoutRow(layout, 0);
			block= uiLayoutGetBlock(row);
				but= uiDefButI(block, NUM, B_FMODIFIER_REDRAW, "Poly Order: ", 10,0,width-30,19, &data->poly_order, 1, 100, 0, 0, "'Order' of the Polynomial - for a polynomial with n terms, 'order' is n-1");
				uiButSetFunc(but, validate_fmodifier_cb, fcm, NULL);
			
			
			/* draw controls for each coefficient and a + sign at end of row */
			row= uiLayoutRow(layout, 1);
			block= uiLayoutGetBlock(row);
				uiDefBut(block, LABEL, 1, "y = ", 0, 0, 50, 20, NULL, 0.0, 0.0, 0, 0, "");
			
			cp= data->coefficients;
			for (i=0; (i < data->arraysize) && (cp); i++, cp++) {
				/* coefficient */
				uiDefButF(block, NUM, B_FMODIFIER_REDRAW, "", 0, 0, 150, 20, cp, -UI_FLT_MAX, UI_FLT_MAX, 10, 3, "Coefficient for polynomial");
				
				/* 'x' param (and '+' if necessary) */
				if (i) {
					if (i == 1)
						strcpy(xval, "x");
					else
						sprintf(xval, "x^%d", i);
					uiDefBut(block, LABEL, 1, xval, 0, 0, 50, 20, NULL, 0.0, 0.0, 0, 0, "Power of x");
				}
				
				if ( (i != (data->arraysize - 1)) || ((i==0) && data->arraysize==2) ) {
					uiDefBut(block, LABEL, 1, "+", 0,0 , 30, 20, NULL, 0.0, 0.0, 0, 0, "");
					
					/* next coefficient on a new row */
					row= uiLayoutRow(layout, 1);
					block= uiLayoutGetBlock(row);
				}
			}
		}
			break;
		
		case FCM_GENERATOR_POLYNOMIAL_FACTORISED: /* factorised polynomial expression */
		{
			float *cp = NULL;
			unsigned int i;
			
			/* draw polynomial order selector */
			row= uiLayoutRow(layout, 0);
			block= uiLayoutGetBlock(row);
				but= uiDefButI(block, NUM, B_FMODIFIER_REDRAW, "Poly Order: ", 0,0,width-30,19, &data->poly_order, 1, 100, 0, 0, "'Order' of the Polynomial - for a polynomial with n terms, 'order' is n-1");
				uiButSetFunc(but, validate_fmodifier_cb, fcm, NULL);
			
			
			/* draw controls for each pair of coefficients */
			row= uiLayoutRow(layout, 1);
			block= uiLayoutGetBlock(row);
				uiDefBut(block, LABEL, 1, "y=", 0, 0, 50, 20, NULL, 0.0, 0.0, 0, 0, "");
			
			cp= data->coefficients;
			for (i=0; (i < data->poly_order) && (cp); i++, cp+=2) {
				/* opening bracket */
				uiDefBut(block, LABEL, 1, "(", 0, 0, 20, 20, NULL, 0.0, 0.0, 0, 0, "");
				
				/* coefficients */
				uiDefButF(block, NUM, B_FMODIFIER_REDRAW, "", 0, 0, 100, 20, cp, -UI_FLT_MAX, UI_FLT_MAX, 10, 3, "Coefficient of x");
				
				uiDefBut(block, LABEL, 1, "x+", 0, 0, 40, 20, NULL, 0.0, 0.0, 0, 0, "");
				
				uiDefButF(block, NUM, B_FMODIFIER_REDRAW, "", 0, 0, 100, 20, cp+1, -UI_FLT_MAX, UI_FLT_MAX, 10, 3, "Second coefficient");
				
				/* closing bracket and '+' sign */
				if ( (i != (data->poly_order - 1)) || ((i==0) && data->poly_order==2) ) {
					uiDefBut(block, LABEL, 1, ") +", 0, 0, 30, 20, NULL, 0.0, 0.0, 0, 0, "");
					
					/* set up new row for the next pair of coefficients*/
					row= uiLayoutRow(layout, 1);
					block= uiLayoutGetBlock(row);
				}
				else 
					uiDefBut(block, LABEL, 1, ")", 0, 0, 20, 20, NULL, 0.0, 0.0, 0, 0, "");
			}
		}
			break;
	}
}

/* --------------- */

/* draw settings for noise modifier */
static void draw_modifier__fn_generator(uiLayout *layout, ID *id, FModifier *fcm, short width)
{
	uiLayout *col;
	PointerRNA ptr;
	
	/* init the RNA-pointer */
	RNA_pointer_create(id, &RNA_FModifierFunctionGenerator, fcm, &ptr);
	
	/* add the settings */
	col= uiLayoutColumn(layout, 1);
		uiItemR(col, "", 0, &ptr, "function_type", 0);
		uiItemR(col, NULL, 0, &ptr, "additive", UI_ITEM_R_TOGGLE);
	
	col= uiLayoutColumn(layout, 0); // no grouping for now
		uiItemR(col, NULL, 0, &ptr, "amplitude", 0);
		uiItemR(col, NULL, 0, &ptr, "phase_multiplier", 0);
		uiItemR(col, NULL, 0, &ptr, "phase_offset", 0);
		uiItemR(col, NULL, 0, &ptr, "value_offset", 0);
}

/* --------------- */

/* draw settings for cycles modifier */
static void draw_modifier__cycles(uiLayout *layout, ID *id, FModifier *fcm, short width)
{
	uiLayout *split, *col;
	PointerRNA ptr;
	
	/* init the RNA-pointer */
	RNA_pointer_create(id, &RNA_FModifierCycles, fcm, &ptr);
	
	/* split into 2 columns 
	 * NOTE: the mode comboboxes shouldn't get labels, otherwise there isn't enough room
	 */
	split= uiLayoutSplit(layout, 0.5f);
	
	/* before range */
	col= uiLayoutColumn(split, 1);
	uiItemL(col, "Before:", 0);
	uiItemR(col, "", 0, &ptr, "before_mode", 0);
	uiItemR(col, NULL, 0, &ptr, "before_cycles", 0);
		
	/* after range */
	col= uiLayoutColumn(split, 1);
	uiItemL(col, "After:", 0);
	uiItemR(col, "", 0, &ptr, "after_mode", 0);
	uiItemR(col, NULL, 0, &ptr, "after_cycles", 0);
}

/* --------------- */

/* draw settings for noise modifier */
static void draw_modifier__noise(uiLayout *layout, ID *id, FModifier *fcm, short width)
{
	uiLayout *split, *col;
	PointerRNA ptr;
	
	/* init the RNA-pointer */
	RNA_pointer_create(id, &RNA_FModifierNoise, fcm, &ptr);
	
	/* blending mode */
	uiItemR(layout, NULL, 0, &ptr, "modification", 0);
	
	/* split into 2 columns */
	split= uiLayoutSplit(layout, 0.5f);
	
	/* col 1 */
	col= uiLayoutColumn(split, 0);
	uiItemR(col, NULL, 0, &ptr, "size", 0);
	uiItemR(col, NULL, 0, &ptr, "strength", 0);
	
	/* col 2 */
	col= uiLayoutColumn(split, 0);
	uiItemR(col, NULL, 0, &ptr, "phase", 0);
	uiItemR(col, NULL, 0, &ptr, "depth", 0);
}

/* --------------- */

#define BINARYSEARCH_FRAMEEQ_THRESH	0.0001

/* Binary search algorithm for finding where to insert Envelope Data Point.
 * Returns the index to insert at (data already at that index will be offset if replace is 0)
 */
static int binarysearch_fcm_envelopedata_index (FCM_EnvelopeData array[], float frame, int arraylen, short *exists)
{
	int start=0, end=arraylen;
	int loopbreaker= 0, maxloop= arraylen * 2;
	
	/* initialise exists-flag first */
	*exists= 0;
	
	/* sneaky optimisations (don't go through searching process if...):
	 *	- keyframe to be added is to be added out of current bounds
	 *	- keyframe to be added would replace one of the existing ones on bounds
	 */
	if ((arraylen <= 0) || (array == NULL)) {
		printf("Warning: binarysearch_fcm_envelopedata_index() encountered invalid array \n");
		return 0;
	}
	else {
		/* check whether to add before/after/on */
		float framenum;
		
		/* 'First' Point (when only one point, this case is used) */
		framenum= array[0].time;
		if (IS_EQT(frame, framenum, BINARYSEARCH_FRAMEEQ_THRESH)) {
			*exists = 1;
			return 0;
		}
		else if (frame < framenum)
			return 0;
			
		/* 'Last' Point */
		framenum= array[(arraylen-1)].time;
		if (IS_EQT(frame, framenum, BINARYSEARCH_FRAMEEQ_THRESH)) {
			*exists= 1;
			return (arraylen - 1);
		}
		else if (frame > framenum)
			return arraylen;
	}
	
	
	/* most of the time, this loop is just to find where to put it
	 * 	- 'loopbreaker' is just here to prevent infinite loops 
	 */
	for (loopbreaker=0; (start <= end) && (loopbreaker < maxloop); loopbreaker++) {
		/* compute and get midpoint */
		int mid = start + ((end - start) / 2);	/* we calculate the midpoint this way to avoid int overflows... */
		float midfra= array[mid].time;
		
		/* check if exactly equal to midpoint */
		if (IS_EQT(frame, midfra, BINARYSEARCH_FRAMEEQ_THRESH)) {
			*exists = 1;
			return mid;
		}
		
		/* repeat in upper/lower half */
		if (frame > midfra)
			start= mid + 1;
		else if (frame < midfra)
			end= mid - 1;
	}
	
	/* print error if loop-limit exceeded */
	if (loopbreaker == (maxloop-1)) {
		printf("Error: binarysearch_fcm_envelopedata_index() was taking too long \n");
		
		// include debug info 
		printf("\tround = %d: start = %d, end = %d, arraylen = %d \n", loopbreaker, start, end, arraylen);
	}
	
	/* not found, so return where to place it */
	return start;
}

/* callback to add new envelope data point */
// TODO: should we have a separate file for things like this?
static void fmod_envelope_addpoint_cb (bContext *C, void *fcm_dv, void *dummy)
{
	Scene *scene= CTX_data_scene(C);
	FMod_Envelope *env= (FMod_Envelope *)fcm_dv;
	FCM_EnvelopeData *fedn;
	FCM_EnvelopeData fed;
	
	/* init template data */
	fed.min= -1.0f;
	fed.max= 1.0f;
	fed.time= (float)scene->r.cfra; // XXX make this int for ease of use?
	fed.f1= fed.f2= 0;
	
	/* check that no data exists for the current frame... */
	if (env->data) {
		short exists = -1;
		int i= binarysearch_fcm_envelopedata_index(env->data, (float)(scene->r.cfra), env->totvert, &exists);
		
		/* binarysearch_...() will set exists by default to 0, so if it is non-zero, that means that the point exists already */
		if (exists)
			return;
			
		/* add new */
		fedn= MEM_callocN((env->totvert+1)*sizeof(FCM_EnvelopeData), "FCM_EnvelopeData");
		
		/* add the points that should occur before the point to be pasted */
		if (i > 0)
			memcpy(fedn, env->data, i*sizeof(FCM_EnvelopeData));
		
		/* add point to paste at index i */
		*(fedn + i)= fed;
		
		/* add the points that occur after the point to be pasted */
		if (i < env->totvert) 
			memcpy(fedn+i+1, env->data+i, (env->totvert-i)*sizeof(FCM_EnvelopeData));
		
		/* replace (+ free) old with new */
		MEM_freeN(env->data);
		env->data= fedn;
		
		env->totvert++;
	}
	else {
		env->data= MEM_callocN(sizeof(FCM_EnvelopeData), "FCM_EnvelopeData");
		*(env->data)= fed;
		
		env->totvert= 1;
	}
}

/* callback to remove envelope data point */
// TODO: should we have a separate file for things like this?
static void fmod_envelope_deletepoint_cb (bContext *C, void *fcm_dv, void *ind_v)
{
	FMod_Envelope *env= (FMod_Envelope *)fcm_dv;
	FCM_EnvelopeData *fedn;
	int index= GET_INT_FROM_POINTER(ind_v);
	
	/* check that no data exists for the current frame... */
	if (env->totvert > 1) {
		/* allocate a new smaller array */
		fedn= MEM_callocN(sizeof(FCM_EnvelopeData)*(env->totvert-1), "FCM_EnvelopeData");
		
		memcpy(fedn, &env->data, sizeof(FCM_EnvelopeData)*(index));
		memcpy(&fedn[index], &env->data[index+1], sizeof(FCM_EnvelopeData)*(env->totvert-index-1));
		
		/* free old array, and set the new */
		MEM_freeN(env->data);
		env->data= fedn;
		env->totvert--;
	}
	else {
		/* just free array, since the only vert was deleted */
		if (env->data) 
			MEM_freeN(env->data);
		env->totvert= 0;
	}
}

/* draw settings for envelope modifier */
static void draw_modifier__envelope(uiLayout *layout, ID *id, FModifier *fcm, short width)
{
	FMod_Envelope *env= (FMod_Envelope *)fcm->data;
	FCM_EnvelopeData *fed;
	uiLayout *col, *row;
	uiBlock *block;
	uiBut *but;
	PointerRNA ptr;
	int i;
	
	/* init the RNA-pointer */
	RNA_pointer_create(id, &RNA_FModifierEnvelope, fcm, &ptr);
	
	/* general settings */
	col= uiLayoutColumn(layout, 1);
		uiItemL(col, "Envelope:", 0);
		uiItemR(col, NULL, 0, &ptr, "reference_value", 0);
		
		row= uiLayoutRow(col, 1);
			uiItemR(row, "Min", 0, &ptr, "default_minimum", 0);
			uiItemR(row, "Max", 0, &ptr, "default_maximum", 0);
			
	/* control points header */
	// TODO: move this control-point control stuff to using the new special widgets for lists
	// the current way is far too cramped
	row= uiLayoutRow(layout, 0);
	block= uiLayoutGetBlock(row);
		
		uiDefBut(block, LABEL, 1, "Control Points:", 0, 0, 150, 20, NULL, 0.0, 0.0, 0, 0, "");
		
		but= uiDefBut(block, BUT, B_FMODIFIER_REDRAW, "Add Point", 0,0,150,19, NULL, 0, 0, 0, 0, "Adds a new control-point to the envelope on the current frame");
		uiButSetFunc(but, fmod_envelope_addpoint_cb, env, NULL);
		
	/* control points list */
	for (i=0, fed=env->data; i < env->totvert; i++, fed++) {
		/* get a new row to operate on */
		row= uiLayoutRow(layout, 1);
		block= uiLayoutGetBlock(row);
		
		uiBlockBeginAlign(block);
			but=uiDefButF(block, NUM, B_FMODIFIER_REDRAW, "Fra:", 0, 0, 90, 20, &fed->time, -UI_FLT_MAX, UI_FLT_MAX, 10, 1, "Frame that envelope point occurs");
			uiButSetFunc(but, validate_fmodifier_cb, fcm, NULL);
			
			uiDefButF(block, NUM, B_FMODIFIER_REDRAW, "Min:", 0, 0, 100, 20, &fed->min, -UI_FLT_MAX, UI_FLT_MAX, 10, 2, "Minimum bound of envelope at this point");
			uiDefButF(block, NUM, B_FMODIFIER_REDRAW, "Max:", 0, 0, 100, 20, &fed->max, -UI_FLT_MAX, UI_FLT_MAX, 10, 2, "Maximum bound of envelope at this point");
			
			but= uiDefIconBut(block, BUT, B_FMODIFIER_REDRAW, ICON_X, 0, 0, 18, 20, NULL, 0.0, 0.0, 0.0, 0.0, "Delete envelope control point");
			uiButSetFunc(but, fmod_envelope_deletepoint_cb, env, SET_INT_IN_POINTER(i));
		uiBlockBeginAlign(block);
	}
}

/* --------------- */

/* draw settings for limits modifier */
static void draw_modifier__limits(uiLayout *layout, ID *id, FModifier *fcm, short width)
{
	uiLayout *split, *col, *row;
	PointerRNA ptr;
	
	/* init the RNA-pointer */
	RNA_pointer_create(id, &RNA_FModifierLimits, fcm, &ptr);
	
	/* row 1: minimum */
	{
		row= uiLayoutRow(layout, 0);
		
		/* split into 2 columns */
		split= uiLayoutSplit(layout, 0.5f);
		
		/* x-minimum */
		col= uiLayoutColumn(split, 1);
		uiItemR(col, NULL, 0, &ptr, "use_minimum_x", 0);
		uiItemR(col, NULL, 0, &ptr, "minimum_x", 0);
			
		/* y-minimum*/
		col= uiLayoutColumn(split, 1);
		uiItemR(col, NULL, 0, &ptr, "use_minimum_y", 0);
		uiItemR(col, NULL, 0, &ptr, "minimum_y", 0);
	}
	
	/* row 2: minimum */
	{
		row= uiLayoutRow(layout, 0);
		
		/* split into 2 columns */
		split= uiLayoutSplit(layout, 0.5f);
		
		/* x-minimum */
		col= uiLayoutColumn(split, 1);
		uiItemR(col, NULL, 0, &ptr, "use_maximum_x", 0);
		uiItemR(col, NULL, 0, &ptr, "maximum_x", 0);
			
		/* y-minimum*/
		col= uiLayoutColumn(split, 1);
		uiItemR(col, NULL, 0, &ptr, "use_maximum_y", 0);
		uiItemR(col, NULL, 0, &ptr, "maximum_y", 0);
	}
}

/* --------------- */


void ANIM_uiTemplate_fmodifier_draw (uiLayout *layout, ID *id, ListBase *modifiers, FModifier *fcm)
{
	FModifierTypeInfo *fmi= fmodifier_get_typeinfo(fcm);
	uiLayout *box, *row, *subrow;
	uiBlock *block;
	uiBut *but;
	short width= 314;
	
	/* draw header */
	{
		/* get layout-row + UI-block for this */
		box= uiLayoutBox(layout);
		
		row= uiLayoutRow(box, 0);
		block= uiLayoutGetBlock(row); // err...
		
		uiBlockSetEmboss(block, UI_EMBOSSN);
		
		/* left-align -------------------------------------------- */
		subrow= uiLayoutRow(row, 0);
		uiLayoutSetAlignment(subrow, UI_LAYOUT_ALIGN_LEFT);
		
		/* expand */
		uiDefIconButBitS(block, ICONTOG, FMODIFIER_FLAG_EXPANDED, B_REDR, ICON_TRIA_RIGHT,	0, -1, UI_UNIT_X, UI_UNIT_Y, &fcm->flag, 0.0, 0.0, 0, 0, "Modifier is expanded.");
		
		/* checkbox for 'active' status (for now) */
		but= uiDefIconButBitS(block, ICONTOG, FMODIFIER_FLAG_ACTIVE, B_REDR, ICON_RADIOBUT_OFF,	0, -1, UI_UNIT_X, UI_UNIT_Y, &fcm->flag, 0.0, 0.0, 0, 0, "Modifier is active one.");
		uiButSetFunc(but, activate_fmodifier_cb, modifiers, fcm);
		
		/* name */
		if (fmi)
			uiDefBut(block, LABEL, 1, fmi->name,	0, 0, 150, UI_UNIT_Y, NULL, 0.0, 0.0, 0, 0, "F-Curve Modifier Type. Click to make modifier active one.");
		else
			uiDefBut(block, LABEL, 1, "<Unknown Modifier>",	0, 0, 150, UI_UNIT_Y, NULL, 0.0, 0.0, 0, 0, "F-Curve Modifier Type. Click to make modifier active one.");
		
		/* right-align ------------------------------------------- */
		subrow= uiLayoutRow(row, 0);
		uiLayoutSetAlignment(subrow, UI_LAYOUT_ALIGN_RIGHT);
		
		/* 'mute' button */
		uiDefIconButBitS(block, ICONTOG, FMODIFIER_FLAG_MUTED, B_REDR, ICON_MUTE_IPO_OFF,	0, 0, UI_UNIT_X, UI_UNIT_Y, &fcm->flag, 0.0, 0.0, 0, 0, "Modifier is temporarily muted (not evaluated).");
		
		/* delete button */
		but= uiDefIconBut(block, BUT, B_REDR, ICON_X, 0, 0, UI_UNIT_X, UI_UNIT_Y, NULL, 0.0, 0.0, 0.0, 0.0, "Delete F-Curve Modifier.");
		uiButSetFunc(but, delete_fmodifier_cb, modifiers, fcm);
		
		uiBlockSetEmboss(block, UI_EMBOSS);
	}
	
	/* when modifier is expanded, draw settings */
	if (fcm->flag & FMODIFIER_FLAG_EXPANDED) {
		/* set up the flexible-box layout which acts as the backdrop for the modifier settings */
		box= uiLayoutBox(layout); 
		
		/* draw settings for individual modifiers */
		switch (fcm->type) {
			case FMODIFIER_TYPE_GENERATOR: /* Generator */
				draw_modifier__generator(box, id, fcm, width);
				break;
				
			case FMODIFIER_TYPE_FN_GENERATOR: /* Built-In Function Generator */
				draw_modifier__fn_generator(box, id, fcm, width);
				break;
				
			case FMODIFIER_TYPE_CYCLES: /* Cycles */
				draw_modifier__cycles(box, id, fcm, width);
				break;
				
			case FMODIFIER_TYPE_ENVELOPE: /* Envelope */
				draw_modifier__envelope(box, id, fcm, width);
				break;
				
			case FMODIFIER_TYPE_LIMITS: /* Limits */
				draw_modifier__limits(box, id, fcm, width);
				break;
			
			case FMODIFIER_TYPE_NOISE: /* Noise */
				draw_modifier__noise(box, id, fcm, width);
				break;
			
			default: /* unknown type */
				break;
		}
	}
}

/* ********************************************** */
