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
//	Rendering main loop and setup functions,
//	 utility functions (BSP, geometry, trigonometry).
//	See tables.c, too.
//



#include <stdlib.h>
#include <math.h>


#include "doomdef.h"
#include "d_net.h"

#include "m_misc.h"

#include "r_local.h"
#include "r_sky.h"


#include "i_system.h"
#include "doomstat.h"


// Fineangles in the SCREENWIDTH wide window.
#define FIELDOFVIEW		2048	




// increment every time a check is made
int16_t			validcount = 1;		


lighttable_t*		fixedcolormap;
extern lighttable_t**	walllights;

int16_t			centerx;
int16_t			centery;

fixed_t			centerxfrac;
fixed_t			centeryfrac;
fixed_t			projection;


fixed_t			viewx;
fixed_t			viewy;
fixed_t			viewz;

angle_t			viewangle;

fixed_t			viewcos;
fixed_t			viewsin;

player_t*		viewplayer;

// 0 = high, 1 = low
int8_t			detailshift;	

//
// precalculated math tables
//
angle_t			clipangle;
angle_t			fieldofview;

// The viewangletox[viewangle + FINEANGLES/4] lookup
// maps the visible view angles to screen X coordinates,
// flattening the arc to a flat projection plane.
// There will be many angles mapped to the same X. 
fixed_t			viewangletox[FINEANGLES/2];

// The xtoviewangleangle[] table maps a screen pixel
// to the lowest viewangle that maps back to x ranges
// from clipangle to -clipangle.
fineangle_t			xtoviewangle[SCREENWIDTH+1];


// UNUSED.
// The finetangentgent[angle+FINEANGLES/4] table
// holds the fixed_t tangent values for view angles,
// ranging from MININT to 0 to MAXINT.
// fixed_t		finetangent[FINEANGLES/2];
// fixed_t		finesine(5*FINEANGLES/4);
//fixed_t*		finecosine = &finesine(FINEANGLES/4);


lighttable_t*		scalelight[LIGHTLEVELS][MAXLIGHTSCALE];
lighttable_t*		scalelightfixed[MAXLIGHTSCALE];
lighttable_t*		zlight[LIGHTLEVELS][MAXLIGHTZ];

// bumped light from gun blasts
uint8_t			extralight;			



void (*colfunc) (void);
void (*basecolfunc) (void);
void (*fuzzcolfunc) (void);
void (*transcolfunc) (void);
void (*spanfunc) (void);

 

//
// R_PointOnSide
// Traverse BSP (sub) tree,
//  check point against partition plane.
// Returns side 0 (front) or 1 (back).
//
int16_t
R_PointOnSide
( fixed_t	x,
  fixed_t	y,
  node_t*	node )
{
    fixed_t	dx;
    fixed_t	dy;
    fixed_t	left;
    fixed_t	right;
	
    if (!node->dx)
    {
	if (x <= node->x)
	    return node->dy > 0;
	
	return node->dy < 0;
    }
    if (!node->dy)
    {
	if (y <= node->y)
	    return node->dx < 0;
	
	return node->dx > 0;
    }
	
    dx = (x - node->x);
    dy = (y - node->y);
	
    // Try to quickly decide by looking at sign bits.
    if ( (node->dy ^ node->dx ^ dx ^ dy)&0x80000000 )
    {
	if  ( (node->dy ^ dx) & 0x80000000 )
	{
	    // (left is negative)
	    return 1;
	}
	return 0;
    }

    left = FixedMul ( node->dy>>FRACBITS , dx );
    right = FixedMul ( dy , node->dx>>FRACBITS );
	
    if (right < left)
    {
	// front side
	return 0;
    }
    // back side
    return 1;			
}


fixed_t
R_PointOnSegSide
( fixed_t	x,
  fixed_t	y,
  int16_t linev1Offset,
	int16_t linev2Offset)
{
    fixed_t	lx;
    fixed_t	ly;
    fixed_t	ldx;
    fixed_t	ldy;
    fixed_t	dx;
    fixed_t	dy;
    fixed_t	left;
    fixed_t	right;
	vertex_t* vertexes = (vertex_t*)Z_LoadBytesFromEMS(vertexesRef);
	
	lx = vertexes[linev1Offset].x;
    ly = vertexes[linev1Offset].y;
	
    ldx = vertexes[linev2Offset].x - lx;
    ldy = vertexes[linev2Offset].y - ly;
	
    if (!ldx)
    {
	if (x <= lx)
	    return ldy > 0;
	
	return ldy < 0;
    }
    if (!ldy)
    {
	if (y <= ly)
	    return ldx < 0;
	
	return ldx > 0;
    }
	
    dx = (x - lx);
    dy = (y - ly);
	
    // Try to quickly decide by looking at sign bits.
    if ( (ldy ^ ldx ^ dx ^ dy)&0x80000000 )
    {
	if  ( (ldy ^ dx) & 0x80000000 )
	{
	    // (left is negative)
	    return 1;
	}
	return 0;
    }

    left = FixedMul ( ldy>>FRACBITS , dx );
    right = FixedMul ( dy , ldx>>FRACBITS );
	
    if (right < left)
    {
	// front side
	return 0;
    }
    // back side
    return 1;			
} 
// todo is this faster for 16 bit?
#define SlopeDiv(num, den) ((den < 512) ? SLOPERANGE : min((num << 3) / (den >> 8), SLOPERANGE))


//
// R_PointToAngle
// To get a global angle from cartesian coordinates,
//  the coordinates are flipped until they are in
//  the first octant of the coordinate system, then
//  the y (<=x) is scaled and divided by x to get a
//  tangent (slope) value which is looked up in the
//  tantoangle[] table.

//




angle_t
R_PointToAngle
( fixed_t	x,
  fixed_t	y )
{	
    x -= viewx;
    y -= viewy;
    
    if ( (!x) && (!y) )
	return 0;

    if (x>= 0)
    {
	// x >=0
	if (y>= 0)
	{
	    // y>= 0

	    if (x>y)
	    {
		// octant 0
		return tantoangle[ SlopeDiv(y,x)];
	    }
	    else
	    {
		// octant 1
		return ANG90-1-tantoangle[ SlopeDiv(x,y)];
	    }
	}
	else
	{
	    // y<0
	    y = -y;

	    if (x>y)
	    {
		// octant 8
		return -tantoangle[SlopeDiv(y,x)];
	    }
	    else
	    {
		// octant 7
		return ANG270+tantoangle[ SlopeDiv(x,y)];
	    }
	}
    }
    else
    {
	// x<0
	x = -x;

	if (y>= 0)
	{
	    // y>= 0
	    if (x>y)
	    {
		// octant 3
		return ANG180-1-tantoangle[ SlopeDiv(y,x)];
	    }
	    else
	    {
		// octant 2
		return ANG90+ tantoangle[ SlopeDiv(x,y)];
	    }
	}
	else
	{
	    // y<0
	    y = -y;

	    if (x>y)
	    {
		// octant 4
		return ANG180+tantoangle[ SlopeDiv(y,x)];
	    }
	    else
	    {
		 // octant 5
		return ANG270-1-tantoangle[ SlopeDiv(x,y)];
	    }
	}
    }
    return 0;
}


angle_t
R_PointToAngle2
( fixed_t	x1,
  fixed_t	y1,
  fixed_t	x2,
  fixed_t	y2 )
{	
    viewx = x1;
    viewy = y1;
    
    return R_PointToAngle (x2, y2);
}


fixed_t
R_PointToDist
( fixed_t	x,
  fixed_t	y )
{
    int16_t		angle;
    fixed_t	dx;
    fixed_t	dy;
    fixed_t	temp;
    fixed_t	dist;
	
    dx = abs(x - viewx);
    dy = abs(y - viewy);
	
    if (dy>dx)
    {
	temp = dx;
	dx = dy;
	dy = temp;
    }
	
    angle = (tantoangle[ FixedDiv(dy,dx)>>DBITS ]+ANG90) >> ANGLETOFINESHIFT;

    // use as cosine
    dist = FixedDiv (dx, finesine(angle) );	
	
    return dist;
}


 

//
// R_ScaleFromGlobalAngle
// Returns the texture mapping scale
//  for the current line (horizontal span)
//  at the given angle.
// rw_distance must be calculated first.
//
fixed_t R_ScaleFromGlobalAngle (angle_t visangle)
{
    fixed_t		scale;
    fineangle_t			anglea;
    fineangle_t			angleb;
    fixed_t			sinea;
    fixed_t			sineb;
    fixed_t		    num;
    fixed_t			den;
	 

    anglea = (ANG90 + (visangle-viewangle))>> ANGLETOFINESHIFT;
    angleb = MOD_FINE_ANGLE(FINE_ANG90 + (visangle >> ANGLETOFINESHIFT) - rw_normalangle);

    // both sines are allways positive
    sinea = finesine(anglea);	
    sineb = finesine(angleb);
    num = FixedMul(projection,sineb)<<detailshift;
    den = FixedMul(rw_distance,sinea);

    if (den > num>>16)
    {
	scale = FixedDiv (num, den);

	if (scale > 64*FRACUNIT)
	    scale = 64*FRACUNIT;
	else if (scale < 256)
	    scale = 256;
    }
    else
	scale = 64*FRACUNIT;
	
    return scale;
}



 

//
// R_InitTextureMapping
//
void R_InitTextureMapping (void)
{
    int16_t			i;
    int16_t			x;
    fixed_t			t;
    fixed_t		focallength;
    
    // Use tangent table to generate viewangletox:
    //  viewangletox will give the next greatest x
    //  after the view angle.
    //
    // Calc focallength
    //  so FIELDOFVIEW angles covers SCREENWIDTH.
    focallength = FixedDiv (centerxfrac,
			    finetangent(FINEANGLES/4+FIELDOFVIEW/2) );
	
    for (i=0 ; i<FINEANGLES/2 ; i++)
    {
	if (finetangent(i) > FRACUNIT*2)
	    t = -1;
	else if (finetangent(i) < -FRACUNIT*2)
	    t = viewwidth+1;
	else
	{
	    t = FixedMul (finetangent(i), focallength);
	    t = (centerxfrac - t+FRACUNIT-1)>>FRACBITS;

	    if (t < -1)
		t = -1;
	    else if (t>viewwidth+1)
		t = viewwidth+1;
	}
	viewangletox[i] = t;
    }
    
    // Scan viewangletox[] to generate xtoviewangle[]:
    //  xtoviewangle will give the smallest view angle
    //  that maps to x.	
    for (x=0;x<=viewwidth;x++) {
        i = 0;
        while (viewangletox[i]>x)
            i++;
        xtoviewangle[x] = MOD_FINE_ANGLE((i)-FINE_ANG90);
    }
    
    // Take out the fencepost cases from viewangletox.
    for (i=0 ; i<FINEANGLES/2 ; i++)
    {
	t = FixedMul (finetangent(i), focallength);
	t = centerx - t;
	
	if (viewangletox[i] == -1)
	    viewangletox[i] = 0;
	else if (viewangletox[i] == viewwidth+1)
	    viewangletox[i]  = viewwidth;
    }
	
    clipangle = xtoviewangle[0] << ANGLETOFINESHIFT;
	fieldofview = 2 * clipangle;
}



//
// R_InitLightTables
// Only inits the zlight table,
//  because the scalelight table changes with view size.
//
#define DISTMAP		2

void R_InitLightTables (void)
{
    int16_t		i;
    int16_t		j;
    int16_t		level;
    int16_t		startmap; 	
    fixed_t		scale;
    
    // Calculate the light levels to use
    //  for each level / distance combination.
    for (i=0 ; i< LIGHTLEVELS ; i++)
    {
	startmap = ((LIGHTLEVELS-1-i)*2)*2; // *NUMCOLORMAPS/LIGHTLEVELS;
	for (j=0 ; j<MAXLIGHTZ ; j++)
	{
	    scale = FixedDiv ((SCREENWIDTH/2*FRACUNIT), (j+1)<<LIGHTZSHIFT);
	    scale >>= LIGHTSCALESHIFT;
	    level = startmap - scale/DISTMAP;
	    
	    if (level < 0)
		    level = 0;

	    if (level >= NUMCOLORMAPS)
		    level = NUMCOLORMAPS-1;

	    zlight[i][j] = colormaps + level*256;
	}
    }
}



//
// R_SetViewSize
// Do not really change anything here,
//  because it might be in the middle of a refresh.
// The change will take effect next refresh.
//
boolean		setsizeneeded;
uint8_t		setblocks;
uint8_t		setdetail;


void
R_SetViewSize
( uint8_t		blocks,
  uint8_t		detail )
{
    setsizeneeded = true;
    setblocks = blocks;
    setdetail = detail;
}


//
// R_ExecuteSetViewSize
//
void R_ExecuteSetViewSize (void)
{
	fixed_t	cosadj;
	fineangle_t	an;
    fixed_t	dy;
    int16_t		i;
    int16_t		j;
    int8_t		level;
    int8_t		startmap; 	

    setsizeneeded = false;

    if (setblocks == 11) {
        scaledviewwidth = SCREENWIDTH;
        viewheight = SCREENHEIGHT;
    } else {
        scaledviewwidth = setblocks*32;
        viewheight = (setblocks*168/10)&~7;
    }
    
    detailshift = setdetail;
    viewwidth = scaledviewwidth>>detailshift;
	
    centery = viewheight/2;
    centerx = viewwidth/2;
    centerxfrac = centerx<<FRACBITS;
    centeryfrac = centery<<FRACBITS;
    projection = centerxfrac;

    if (!detailshift)
    {
	colfunc = basecolfunc = R_DrawColumn;
	fuzzcolfunc = R_DrawFuzzColumn;
	transcolfunc = R_DrawTranslatedColumn;
	spanfunc = R_DrawSpan;
    }
    else
    {
	colfunc = basecolfunc = R_DrawColumnLow;
	fuzzcolfunc = R_DrawFuzzColumn;
	transcolfunc = R_DrawTranslatedColumn;
	spanfunc = R_DrawSpanLow;
    }

    R_InitBuffer (scaledviewwidth, viewheight);
	
    R_InitTextureMapping ();
    
    // psprite scales
    pspritescale = FRACUNIT*viewwidth/SCREENWIDTH;
    pspriteiscale = FRACUNIT*SCREENWIDTH/viewwidth;
    
    // thing clipping
    for (i=0 ; i<viewwidth ; i++)
	screenheightarray[i] = viewheight;
    
    // planes
    for (i=0 ; i<viewheight ; i++)
    {
	dy = ((i-viewheight/2)<<FRACBITS)+FRACUNIT/2;
	dy = abs(dy);
	yslope[i] = FixedDiv ( (viewwidth<<detailshift)/2*FRACUNIT, dy);
    }
	
    for (i=0 ; i<viewwidth ; i++)
    {
		an = xtoviewangle[i] ;
		cosadj = abs(finecosine(an));
		distscale[i] = FixedDiv (FRACUNIT,cosadj);
    }
    
    // Calculate the light levels to use
    //  for each level / scale combination.
    for (i=0 ; i< LIGHTLEVELS ; i++)
    {
	startmap = ((LIGHTLEVELS-1-i)*2)*NUMCOLORMAPS/LIGHTLEVELS;
	for (j=0 ; j<MAXLIGHTSCALE ; j++)
	{
	    level = startmap - j*SCREENWIDTH/(viewwidth<<detailshift)/DISTMAP;
	    
	    if (level < 0)
		level = 0;

	    if (level >= NUMCOLORMAPS)
		level = NUMCOLORMAPS-1;

	    scalelight[i][j] = colormaps + level*256;
	}
    }
}



//
// R_Init
//
extern uint8_t	detailLevel;
extern uint8_t	screenblocks;



void R_Init (void)
{
    R_InitData ();
    printf (".");
    // viewwidth / viewheight / detailLevel are set by the defaults
    printf (".");

    R_SetViewSize (screenblocks, detailLevel);
    R_InitPlanes ();
    printf (".");
    R_InitLightTables ();
    printf (".");
    R_InitSkyMap ();
    printf (".");
    R_InitTranslationTables ();
	

}


//
// R_PointInSubsector
//
int16_t
 R_PointInSubsector
( fixed_t	x,
  fixed_t	y )
{
    node_t*	node;
    int16_t		side;
    int16_t		nodenum;
	node_t* nodes;
    // single subsector is a special case
	if (!numnodes) {
		return 0;
	}
		
    nodenum = numnodes-1;
	nodes = (node_t*)Z_LoadBytesFromEMS(nodesRef);
	while (! (nodenum & NF_SUBSECTOR) ) {
		node = &nodes[nodenum];
		side = R_PointOnSide (x, y, node);
		nodenum = node->children[side];

    }

	return nodenum & ~NF_SUBSECTOR;
}



//
// R_SetupFrame
//
void R_SetupFrame (player_t* player)
{		
    int8_t		i;
	fixed_t tempan;
	mobj_t* playermo = (mobj_t*)Z_LoadBytesFromEMS(player->moRef);

    viewplayer = player;
    viewx = playermo->x;
    viewy = playermo->y;
    viewangle = playermo->angle;
    extralight = player->extralight;

    viewz = player->viewz;
	tempan = viewangle >> ANGLETOFINESHIFT;
    viewsin = finesine(tempan);
    viewcos = finecosine(tempan);
	
    if (player->fixedcolormap)
    {
	fixedcolormap =
	    colormaps
	    + player->fixedcolormap*256*sizeof(lighttable_t);
	
	walllights = scalelightfixed;

	for (i=0 ; i<MAXLIGHTSCALE ; i++)
	    scalelightfixed[i] = fixedcolormap;
    }
    else
	fixedcolormap = 0;
		
    validcount++;
    destview = destscreen + (viewwindowy*SCREENWIDTH/4) + (viewwindowx >> 2);
}



//
// R_RenderView
//
void R_RenderPlayerView (player_t* player)
{	

	R_SetupFrame (player);

    // Clear buffers.
    R_ClearClipSegs ();
    R_ClearDrawSegs ();
    R_ClearPlanes ();
    R_ClearSprites ();
    
    // check for new console commands.
    NetUpdate ();


    // The head node is the last node output.
	Z_LoadBytesFromEMS(nodesRef);

	R_RenderBSPNode (numnodes-1);

    // Check for new console commands.
    NetUpdate ();
    
    R_DrawPlanes ();
    
    // Check for new console commands.
    NetUpdate ();
    
    R_DrawMasked ();

    // Check for new console commands.
    NetUpdate ();				
}
