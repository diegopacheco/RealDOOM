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
//	Refresh/render internal state variables (global).
//


#ifndef __R_STATE__
#define __R_STATE__

// Need data structure definitions.
#include "d_player.h"
#include "r_data.h"
#include "z_zone.h"


#define SECNUM_NULL -1
#define LINENUM_NULL -1
//
// Refresh internal data structures,
//  for rendering.
//

// needed for texture pegging
extern MEMREF		textureheightRef;

// needed for pre rendering (fracs)
extern MEMREF		spritewidthRef;

extern MEMREF		spriteoffsetRef;
extern MEMREF		spritetopoffsetRef;

//extern MEMREF		colormapsRef;
extern lighttable_t* colormaps;

extern int		viewwidth;
extern int		scaledviewwidth;
extern int		viewheight;

extern int		firstflat;

// for global animation
extern MEMREF	flattranslationRef;	
extern MEMREF	texturetranslationRef;	


// Sprite....
extern int		firstspritelump;
extern int		lastspritelump;
extern int		numspritelumps;



//
// Lookup tables for map data.
//
extern int		numsprites;
extern MEMREF	spritesRef;

extern int		numvertexes;
//extern vertex_t vertexes[946];
extern MEMREF	vertexesRef;

extern int		numsegs;
extern MEMREF		segsRef;

extern int		numsectors;
extern MEMREF	sectorsRef;

extern int		numsubsectors;
extern MEMREF	subsectorsRef;

extern int		numnodes;
extern MEMREF    nodesRef;


extern int		numlines;
extern MEMREF   linesRef;

extern int		numsides;
extern MEMREF       sidesRef;

//extern short*	linebuffer;
extern MEMREF          linebufferRef;


//
// POV data.
//
extern fixed_t		viewx;
extern fixed_t		viewy;
extern fixed_t		viewz;

extern angle_t		viewangle;
extern player_t*	viewplayer;


// ?
extern angle_t		clipangle;

extern int		viewangletox[FINEANGLES/2];
extern angle_t		xtoviewangle[SCREENWIDTH+1];
//extern fixed_t		finetangent[FINEANGLES/2];

extern fixed_t		rw_distance;
extern angle_t		rw_normalangle;



// angle to line origin
extern int		rw_angle1;

// Segs count?
extern int		sscount;

extern visplane_t*	floorplane;
extern visplane_t*	ceilingplane;


#endif
