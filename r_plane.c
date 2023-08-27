//
// Copyright (C) 1993-1996 Id Software, Inc.
// Copyright (C) 2016-2017 Alexey Khokholov (Nuke.YKT)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Here is a core component: drawing the floors and ceilings,
//	 while maintaining a per column clipping list only.
//	Moreover, the sky areas have to be determined.
//

#include <stdlib.h>

#include "i_system.h"
#include "z_zone.h"
#include "w_wad.h"

#include "doomdef.h"
#include "doomstat.h"

#include "r_local.h"
#include "r_sky.h"



planefunction_t		floorfunc;
planefunction_t		ceilingfunc;

//
// opening
//

// Here comes the obnoxious "visplane".
#define MAXVISPLANES	128
visplane_t		visplanes[MAXVISPLANES];
visplane_t*		lastvisplane;
visplane_t*		floorplane;
visplane_t*		ceilingplane;

// ?
#define MAXOPENINGS	SCREENWIDTH*64
int16_t			openings[MAXOPENINGS];
int16_t*			lastopening;


//
// Clip values are the solid pixel bounding the range.
//  floorclip starts out SCREENHEIGHT
//  ceilingclip starts out -1
//
int16_t			floorclip[SCREENWIDTH];
int16_t			ceilingclip[SCREENWIDTH];

//
// spanstart holds the start of a plane span
// initialized to 0 at start
//
int16_t			spanstart[SCREENHEIGHT];
//int32_t			spanstop[SCREENHEIGHT];

//
// texture mapping
//
lighttable_t**		planezlight;
fixed_t			planeheight;

fixed_t			yslope[SCREENHEIGHT];
fixed_t			distscale[SCREENWIDTH];
fixed_t			basexscale;
fixed_t			baseyscale;

fixed_t			cachedheight[SCREENHEIGHT];
fixed_t			cacheddistance[SCREENHEIGHT];
fixed_t			cachedxstep[SCREENHEIGHT];
fixed_t			cachedystep[SCREENHEIGHT];



//
// R_InitPlanes
// Only at game startup.
//
void R_InitPlanes (void)
{
  // Doh!
}


//
// R_MapPlane
//
// Uses global vars:
//  planeheight
//  ds_source
//  basexscale
//  baseyscale
//  viewx
//  viewy
//
// BASIC PRIMITIVE
//
void
R_MapPlane
( int32_t		y,
  int32_t		x1,
  int32_t		x2 )
{
    fineangle_t	angle;
    fixed_t	distance;
    fixed_t	length;
	uint32_t	index;
 

    if (planeheight != cachedheight[y])
    {
	cachedheight[y] = planeheight;
	distance = cacheddistance[y] = FixedMul (planeheight, yslope[y]);
	ds_xstep = cachedxstep[y] = FixedMul (distance,basexscale);
	ds_ystep = cachedystep[y] = FixedMul (distance,baseyscale);
    }
    else
    {
	distance = cacheddistance[y];
	ds_xstep = cachedxstep[y];
	ds_ystep = cachedystep[y];
    }
	
    length = FixedMul (distance,distscale[x1]);
	angle = MOD_FINE_ANGLE((viewangle >>ANGLETOFINESHIFT)+ xtoviewangle[x1]);

    ds_xfrac = viewx + FixedMul(finecosine(angle), length);
    ds_yfrac = -viewy - FixedMul(finesine(angle), length);

    if (fixedcolormap)
	ds_colormap = fixedcolormap;
    else
    {
	index = distance >> LIGHTZSHIFT;
	
	if (index >= MAXLIGHTZ )
	    index = MAXLIGHTZ-1;

	ds_colormap = planezlight[index];
    }
	
    ds_y = y;
    ds_x1 = x1;
    ds_x2 = x2;

    // high or low detail
    spanfunc ();	
}


//
// R_ClearPlanes
// At begining of frame.
//
void R_ClearPlanes (void)
{
    int16_t		i;
    fineangle_t	angle;
    
    // opening / clipping determination
    for (i=0 ; i<viewwidth ; i++)
    {
	floorclip[i] = viewheight;
	ceilingclip[i] = -1;
    }

    lastvisplane = visplanes;
    lastopening = openings;
    
    // texture calculation
    memset (cachedheight, 0, sizeof(cachedheight));

    // left to right mapping
    angle = (viewangle-ANG90)>>ANGLETOFINESHIFT;
	
    // scale will be unit scale at SCREENWIDTH/2 distance
    basexscale = FixedDiv (finecosine(angle),centerxfrac);
    baseyscale = -FixedDiv (finesine(angle),centerxfrac);
}




//
// R_FindPlane
//
visplane_t*
R_FindPlane
( fixed_t	height,
  uint8_t		picnum,
  uint8_t		lightlevel )
{
    visplane_t*	check;
	
    if (picnum == skyflatnum)
    {
	height = 0;			// all skys map together
	lightlevel = 0;
    }
	
    for (check=visplanes; check<lastvisplane; check++)
    {
	if (height == check->height
	    && picnum == check->picnum
	    && lightlevel == check->lightlevel)
	{
	    break;
	}
    }
    
			
    if (check < lastvisplane)
	return check;
		
    if (lastvisplane - visplanes == MAXVISPLANES)
	I_Error ("R_FindPlane: no more visplanes");
		
    lastvisplane++;

    check->height = height;
    check->picnum = picnum;
    check->lightlevel = lightlevel;
    check->minx = SCREENWIDTH;
    check->maxx = -1;
    
    memset (check->top,0xff,sizeof(check->top));
		
    return check;
}


//
// R_CheckPlane
//
visplane_t*
R_CheckPlane
( visplane_t*	pl,
  int16_t		start,
  int16_t		stop )
{
    int16_t		intrl;
    int16_t		intrh;
    int16_t		unionl;
    int16_t		unionh;
    int16_t		x;
	
    if (start < pl->minx)
    {
	intrl = pl->minx;
	unionl = start;
    }
    else
    {
	unionl = pl->minx;
	intrl = start;
    }
	
    if (stop > pl->maxx)
    {
	intrh = pl->maxx;
	unionh = stop;
    }
    else
    {
	unionh = pl->maxx;
	intrh = stop;
    }

    for (x=intrl ; x<= intrh ; x++)
	if (pl->top[x] != 0xff)
	    break;

    if (x > intrh)
    {
	pl->minx = unionl;
	pl->maxx = unionh;

	// use the same one
	return pl;		
    }
	
    // make a new visplane
    lastvisplane->height = pl->height;
    lastvisplane->picnum = pl->picnum;
    lastvisplane->lightlevel = pl->lightlevel;
    
    pl = lastvisplane++;
    pl->minx = start;
    pl->maxx = stop;

    memset (pl->top,0xff,sizeof(pl->top));
		
    return pl;
}




//
// R_DrawPlanes
// At the end of each frame.
//
void R_DrawPlanes (void)
{
    visplane_t*		pl;
    uint8_t			light;
    int16_t			x;
    int16_t			stop;
    fineangle_t			angle;
	uint8_t * flattranslation;
	byte t1, b1, t2, b2;

 
    for (pl = visplanes ; pl < lastvisplane ; pl++)
    {
	if (pl->minx > pl->maxx)
	    continue;

	
	// sky flat
	if (pl->picnum == skyflatnum)
	{
	    dc_iscale = pspriteiscale>>detailshift;
	    
	    // Sky is allways drawn full bright,
	    //  i.e. colormaps[0] is used.
	    // Because of this hack, sky is not affected
	    //  by INVUL inverse mapping.
	    dc_colormap = colormaps;
	    dc_texturemid = 100*FRACUNIT; // this was hardcoded as the only use of skytexturemid..
	    for (x=pl->minx ; x <= pl->maxx ; x++)
	    {
		dc_yl = pl->top[x];
		dc_yh = pl->bottom[x];

		if (dc_yl <= dc_yh)
		{
		    angle = MOD_FINE_ANGLE((viewangle + (xtoviewangle[x] << ANGLETOFINESHIFT)) >> ANGLETOSKYSHIFT) ;
		    dc_x = x;

			dc_source = R_GetColumn(skytexture, angle);
		    colfunc ();
		}
	    }
	    continue;
	}
	
	flattranslation = Z_LoadBytesFromEMS(flattranslationRef);
	// regular flat
	ds_sourceRef = W_CacheLumpNumEMS(firstflat +
		flattranslation[pl->picnum],
		PU_STATIC);
	ds_source = Z_LoadBytesFromEMS(ds_sourceRef);
	
	planeheight = abs(pl->height-viewz);
	light = (pl->lightlevel >> LIGHTSEGSHIFT)+extralight;

	if (light >= LIGHTLEVELS)
	    light = LIGHTLEVELS-1;
 

	planezlight = zlight[light];

	pl->top[pl->maxx+1] = 0xff;
	pl->top[pl->minx-1] = 0xff;
		
	stop = pl->maxx + 1;

	for (x=pl->minx ; x<= stop ; x++)
	{
			t1 = pl->top[x - 1];
			b1 = pl->bottom[x - 1];
			t2 = pl->top[x];
			b2 = pl->bottom[x];


		while (t1 < t2 && t1 <= b1)
		{
			R_MapPlane(t1, spanstart[t1], x - 1);
			t1++;
		}
		while (b1 > b2 && b1 >= t1)
		{
			R_MapPlane(b1, spanstart[b1], x - 1);
			b1--;
		}

		while (t2 < t1 && t2 <= b2)
		{
			spanstart[t2] = x;
			t2++;
		}
		while (b2 > b1 && b2 >= t2)
		{
			spanstart[b2] = x;
			b2--;
		}

	}
	
	Z_ChangeTagEMSNew (ds_sourceRef, PU_CACHE);
    }
}

