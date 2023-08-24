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
//	LineOfSight/Visibility checks, uses REJECT Lookup Table.
//


#include "doomdef.h"

#include "i_system.h"
#include "p_local.h"

// State.
#include "r_state.h"
#include "m_misc.h"

//
// P_CheckSight
//
fixed_t		sightzstart;		// eye z of looker
fixed_t		topslope;
fixed_t		bottomslope;		// slopes to top and bottom of target

divline_t	strace;			// from t1 to t2
fixed_t		cachedt2x;
fixed_t		cachedt2y;

int		sightcounts[2];


//
// P_DivlineSide
// Returns side 0 (front), 1 (back), or 2 (on).
//
int
P_DivlineSide
( fixed_t	x,
  fixed_t	y,
  divline_t*	node )
{
    fixed_t	dx;
    fixed_t	dy;
    fixed_t	left;
    fixed_t	right;

    if (!node->dx)
    {
	if (x==node->x)
	    return 2;
	
	if (x <= node->x)
	    return node->dy > 0;

	return node->dy < 0;
    }
    
    if (!node->dy)
    {
	if (x==node->y)
	    return 2;

	if (y <= node->y)
	    return node->dx < 0;

	return node->dx > 0;
    }
	
    dx = (x - node->x);
    dy = (y - node->y);

    left =  (node->dy>>FRACBITS) * (dx>>FRACBITS);
    right = (dy>>FRACBITS) * (node->dx>>FRACBITS);
	
    if (right < left)
	return 0;	// front side
    
    if (left == right)
	return 2;
    return 1;		// back side
}


//
// P_InterceptVector2
// Returns the fractional intercept point
// along the first divline.
// This is only called by the addthings and addlines traversers.
//
fixed_t
P_InterceptVector2
( divline_t*	v2,
  divline_t*	v1 )
{
    fixed_t	frac;
    fixed_t	num;
    fixed_t	den;
	
    den = FixedMul (v1->dy>>8,v2->dx) - FixedMul(v1->dx>>8,v2->dy);

    if (den == 0)
	return 0;
    //	I_Error ("P_InterceptVector: parallel");
    
    num = FixedMul ( (v1->x - v2->x)>>8 ,v1->dy) + 
	FixedMul ( (v2->y - v1->y)>>8 , v1->dx);
    frac = FixedDiv (num , den);

    return frac;
}

static int bspcounter = 0;

//
// P_CrossSubsector
// Returns true
//  if strace crosses the given subsector successfully.
//
boolean P_CrossSubsector (short subsecnum)
{
    short		segnum;
	short linedefOffset;
    line_t*		line;
    int			s1;
    int			s2;
    int			count;
    short frontsecnum;
	short backsecnum;
    fixed_t		opentop;
    fixed_t		openbottom;
    divline_t		divl;
    vertex_t		v1;
    vertex_t		v2;
    fixed_t		frac;
    fixed_t		slope;
	seg_t* segs;
	vertex_t* vertexes;
	line_t* lines;
	short linev1Offset;
	short linev2Offset;
	short lineflags;
	subsector_t* subsectors = (subsector_t*)Z_LoadBytesFromEMS(subsectorsRef);


#ifdef RANGECHECK
	if (subsecnum >= numsubsectors) {
		I_Error("P_CrossSubsector: ss %i with numss = %i %i %i %i %i", subsecnum, numsubsectors, numreads, pageins, pageouts, bspcounter);
	}
#endif
    
    // check lines
    count = subsectors[subsecnum].numlines;
    segnum = subsectors[subsecnum].firstline;
	

	if (setval == 1) {
		//I_Error("check %i %i %i ", count, segnum, subsecnum);
	}

    for ( ; count ; segnum++, count--) {
		segs = (seg_t*)Z_LoadBytesFromEMS(segsRef);
		linedefOffset = segs[segnum].linedefOffset;
		frontsecnum = segs[segnum].frontsecnum;
		backsecnum = segs[segnum].backsecnum;
		lines = (line_t*)Z_LoadBytesFromEMS(linesRef);
		line = &lines[linedefOffset];



		// allready checked other side?
		if (line->validcount == validcount) {
			continue;
		}
		line->validcount = validcount;
		linev1Offset = line->v1Offset;
		linev2Offset = line->v2Offset;
		lineflags = line->flags;
		if (setval == 1 && count == 5) {
			//			I_Error("check %i %i  %i %i %i %i %i %i %i %i", v1.x, v1.y, v2.x, v2.y, strace.x, strace.y, strace.dx, strace.dy, s1, s2);
			//I_Error("check %i %i  %i %i %i %i %i ", v1.x, v1.y, line->v1Offset, line->v2Offset, linedefOffset, s1, s2);
			// lines[355]->v1Offset is bad

		}

		vertexes = (vertex_t*)Z_LoadBytesFromEMS(vertexesRef);

		v1 = vertexes[linev1Offset];
		v2 = vertexes[linev2Offset];
		s1 = P_DivlineSide (v1.x,v1.y, &strace);
		s2 = P_DivlineSide (v2.x, v2.y, &strace);

		// line isn't crossed?
		if (s1 == s2)
			continue;
	
		divl.x = v1.x;
		divl.y = v1.y;
		divl.dx = v2.x - v1.x;
		divl.dy = v2.y - v1.y;
		s1 = P_DivlineSide (strace.x, strace.y, &divl);
		s2 = P_DivlineSide (cachedt2x, cachedt2y, &divl);

		// line isn't crossed?
		if (s1 == s2)
			continue;	
		

		// stop because it is not two sided anyway
		// might do this after updating validcount?
		if (!(lineflags & ML_TWOSIDED)) {
			if (setval == 1) {
				I_Error("false return C %i", count);
			}

			return false;
		}
	
		// crosses a two sided line

		// no wall to block sight with?
		if (sectors[frontsecnum].floorheight == sectors[backsecnum].floorheight && sectors[frontsecnum].ceilingheight == sectors[backsecnum].ceilingheight) {
			continue;
		}

		// possible occluder
		// because of ceiling height differences
		if (sectors[frontsecnum].ceilingheight < sectors[backsecnum].ceilingheight)
			opentop = sectors[frontsecnum].ceilingheight;
		else
			opentop = sectors[backsecnum].ceilingheight;

		// because of ceiling height differences
		if (sectors[frontsecnum].floorheight > sectors[backsecnum].floorheight)
			openbottom = sectors[frontsecnum].floorheight;
		else
			openbottom = sectors[backsecnum].floorheight;
		
		// quick test for totally closed doors
		if (openbottom >= opentop) {
			if (setval == 1) {
				I_Error("false return A%i", count);
			}

			return false;		// stop
		}
	
		frac = P_InterceptVector2 (&strace, &divl);
		
		if (sectors[frontsecnum].floorheight != sectors[backsecnum].floorheight) {
			slope = FixedDiv (openbottom - sightzstart , frac);
			if (slope > bottomslope)
			bottomslope = slope;
		}
		
		if (sectors[frontsecnum].ceilingheight != sectors[backsecnum].ceilingheight) {
			slope = FixedDiv (opentop - sightzstart , frac);
			if (slope < topslope)
			topslope = slope;
		}
		
		if (topslope <= bottomslope) {
			if (setval == 1) {
				I_Error("false return B%i", count);
			}

			return false;		// stop				
		}
    }
    // passed the subsector ok

	

    return true;		
}


//
// P_CrossBSPNode
// Returns true
//  if strace crosses the given node successfully.
//
boolean P_CrossBSPNode (unsigned short bspnum)
{
    node_t*	bsp;
    int		side;
	node_t* nodes;
	bspcounter++;
	
 	//if (bspnum > numsubsectors) {
		//I_Error("too big %i %i", bspnum&(~NF_SUBSECTOR), numsubsectors, bspnum);
	//}

	if (bspnum & NF_SUBSECTOR) {
		if (bspnum == -1) {
			if (setval == 1) {
				I_Error("here A %i %i", P_CrossSubsector(0), bspnum);
			}
			return P_CrossSubsector(0);
		} else {
			if (setval == 1) {
				I_Error("here B %i %i", P_CrossSubsector(bspnum&(~NF_SUBSECTOR)), bspnum);
			}
			return P_CrossSubsector(bspnum&(~NF_SUBSECTOR));
		}
    }
		

	nodes = (node_t*)Z_LoadBytesFromEMS(nodesRef);
	bsp = &nodes[bspnum];
    
    // decide which side the start point is on
    side = P_DivlineSide (strace.x, strace.y, (divline_t *)bsp);
	if (side == 2) {
		side = 0;	// an "on" should cross both sides
	}
	nodes = (node_t*)Z_LoadBytesFromEMS(nodesRef);
	bsp = &nodes[bspnum];

	if (!P_CrossBSPNode(bsp->children[side])) {
		return false;
	}

	nodes = (node_t*)Z_LoadBytesFromEMS(nodesRef);
	bsp = &nodes[bspnum];

    // the partition plane is crossed here
    if (side == P_DivlineSide (cachedt2x, cachedt2y,(divline_t *)bsp)) {
		// the line doesn't touch the other side
		return true;
    }
	nodes = (node_t*)Z_LoadBytesFromEMS(nodesRef);
	bsp = &nodes[bspnum];

    // cross the ending side		
    return P_CrossBSPNode (bsp->children[side^1]);
}


//
// P_CheckSight
// Returns true
//  if a straight line between t1 and t2 is unobstructed.
// Uses REJECT.
//
boolean
P_CheckSight2
( MEMREF t1Ref,
  MEMREF t2Ref, char* file, int line)
{



    int		pnum;
    int		bytenum;
    int		bitnum;
	mobj_t*	t1 = (mobj_t*)Z_LoadBytesFromEMS(t1Ref);
	fixed_t t1z = t1->z;
	fixed_t t1x = t1->x;
	fixed_t t1y = t1->y;
	fixed_t t1height = t1->height;
	short s1 = t1->secnum;

	mobj_t*	t2;
		fixed_t t2z;
		fixed_t t2x;
		fixed_t t2y;
		fixed_t t2height;
		short s2;

	 

	t2 = (mobj_t*)Z_LoadBytesFromEMS(t2Ref);
	t2z = t2->z;
	t2x = t2->x;
	t2y = t2->y;
	t2height = t2->height;
	s2 = t2->secnum;


	
	//I_Error("set call %i  %i %p %p", t1Ref, t2Ref, t1, t2);

    // First check for trivial rejection.

    // Determine subsector entries in REJECT table.
    pnum = s1*numsectors + s2;
    bytenum = pnum>>3;
    bitnum = 1 << (pnum&7);
	


    // Check in REJECT table.
    if (rejectmatrix[bytenum]&bitnum) {
		sightcounts[0]++;

		// can't possibly be connected
		return false;	
    }

    // An unobstructed LOS is possible.
    // Now look from eyes of t1 to any part of t2.
    sightcounts[1]++;
    validcount++;
	
    sightzstart = t1z + t1height - (t1height>>2);
    topslope = (t2z+t2height) - sightzstart;
    bottomslope = (t2z) - sightzstart;
	
    strace.x = t1x;
    strace.y = t1y;
    cachedt2x = t2x;
	cachedt2y = t2y;
    strace.dx = t2x - t1x;
    strace.dy = t2y - t1y;

	if (setval == 1) {
		I_Error("blah %i %i %i %i %i %i %i  ", prndindex, P_CrossBSPNode(numnodes - 1));
	}

    // the head node is the last node output
    return P_CrossBSPNode (numnodes-1);	
}


