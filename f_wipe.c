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
//	Mission begin melt/wipe screen special effect.
//



#include "z_zone.h"
#include "i_system.h"
#include "v_video.h"
#include "m_misc.h"

#include "doomdef.h"

#include "f_wipe.h"

//
//                       SCREEN WIPE PACKAGE
//

// when zero, stop the wipe
static boolean	go = 0;

static byte*	wipe_scr_start;
static byte*	wipe_scr_end;
static byte*	wipe_scr;


void
wipe_shittyColMajorXform
( int16_t*	array,
  int32_t		width,
  int32_t		height )
{
    int32_t		x;
    int32_t		y;
	MEMREF destRef;
    int16_t*	dest;

	destRef = Z_MallocEMSNew(width*height * 2, PU_STATIC, 0, ALLOC_TYPE_FWIPE);
	dest = (int16_t*)Z_LoadBytesFromEMS(destRef);

    for(y=0;y<height;y++)
	for(x=0;x<width;x++)
	    dest[x*height+y] = array[y*width+x];

    memcpy(array, dest, width*height*2);

    Z_FreeEMSNew(destRef);

}

int
wipe_initColorXForm
( int32_t	width,
  int32_t	height,
  int32_t	ticks )
{
    memcpy(wipe_scr, wipe_scr_start, width*height);
    return 0;
}

int
wipe_doColorXForm
( int32_t	width,
  int32_t	height,
  int32_t	ticks )
{
    boolean	changed;
    byte*	w;
    byte*	e;
    int32_t		newval;

    changed = false;
    w = wipe_scr;
    e = wipe_scr_end;
    
    while (w!=wipe_scr+width*height)
    {
	if (*w != *e)
	{
	    if (*w > *e)
	    {
		newval = *w - ticks;
		if (newval < *e)
		    *w = *e;
		else
		    *w = newval;
		changed = true;
	    }
	    else if (*w < *e)
	    {
		newval = *w + ticks;
		if (newval > *e)
		    *w = *e;
		else
		    *w = newval;
		changed = true;
	    }
	}
	w++;
	e++;
    }

    return !changed;

}

int
wipe_exitColorXForm
( int32_t	width,
  int32_t	height,
  int32_t	ticks )
{
    return 0;
}


static MEMREF yRef;

int
wipe_initMelt
( int32_t	width,
  int32_t	height,
  int32_t	ticks )
{
	int32_t i, r;
	int32_t* y;

    // copy start screen to main screen
    memcpy(wipe_scr, wipe_scr_start, width*height);
    
    // makes this wipe faster (in theory)
    // to have stuff in column-major format
    wipe_shittyColMajorXform((int16_t*)wipe_scr_start, width/2, height);
    wipe_shittyColMajorXform((int16_t*)wipe_scr_end, width/2, height);
    
    // setup initial column positions
    // (y<0 => not ready to scroll yet)

	

	yRef = Z_MallocEMSNew(width*sizeof(int), PU_STATIC, 0, ALLOC_TYPE_FWIPE);
	y = (int32_t*)Z_LoadBytesFromEMS(yRef);



    y[0] = -(M_Random()%16);
    for (i=1;i<width;i++)
    {
	r = (M_Random()%3) - 1;
	y[i] = y[i-1] + r;
	if (y[i] > 0) y[i] = 0;
	else if (y[i] == -16) y[i] = -15;
    }

    return 0;
}

int
wipe_doMelt
( int32_t	width,
  int32_t	height,
  int32_t	ticks )
{
    int32_t		i;
    int32_t		j;
    int32_t		dy;
    int32_t		idx;
    
	int32_t* y;
    int16_t*	s;
    int16_t*	d;
    boolean	done = true;

    width/=2;

	y = (int32_t*)Z_LoadBytesFromEMS(yRef);
	while (ticks--)
    {
	for (i=0;i<width;i++)
	{
	    if (y[i]<0)
	    {
		y[i]++; done = false;
	    }
	    else if (y[i] < height)
	    {
		dy = (y[i] < 16) ? y[i]+1 : 8;
		if (y[i]+dy >= height) dy = height - y[i];
		s = &((int16_t *)wipe_scr_end)[i*height+y[i]];
		d = &((int16_t *)wipe_scr)[y[i]*width+i];
		idx = 0;
		for (j=dy;j;j--)
		{
		    d[idx] = *(s++);
		    idx += width;
		}
		y[i] += dy;
		s = &((int16_t *)wipe_scr_start)[i*height];
		d = &((int16_t *)wipe_scr)[y[i]*width+i];
		idx = 0;
		for (j=height-y[i];j;j--)
		{
		    d[idx] = *(s++);
		    idx += width;
		}
		done = false;
	    }
	}
    }

    return done;

}

int
wipe_exitMelt
( int32_t	width,
  int32_t	height,
  int32_t	ticks )
{
	Z_FreeEMSNew(yRef);

    return 0;
}

int
wipe_StartScreen
( int32_t	x,
  int32_t	y,
  int32_t	width,
  int32_t	height )
{
    wipe_scr_start = screens[2];
    I_ReadScreen(wipe_scr_start);
    return 0;
}

int
wipe_EndScreen
( int32_t	x,
  int32_t	y,
  int32_t	width,
  int32_t	height )
{
    wipe_scr_end = screens[3];
    I_ReadScreen(wipe_scr_end);
    V_DrawBlock(x, y, 0, width, height, wipe_scr_start); // restore start scr.
    return 0;
}

int
wipe_ScreenWipe
( int32_t	wipeno,
  int32_t	x,
  int32_t	y,
  int32_t	width,
  int32_t	height,
  int32_t	ticks )
{
	int32_t rc;
    static int32_t(*wipes[])(int, int, int) =
    {
	wipe_initColorXForm, wipe_doColorXForm, wipe_exitColorXForm,
	wipe_initMelt, wipe_doMelt, wipe_exitMelt
    };

    void V_MarkRect(int, int, int, int);

    // initial stuff
    if (!go)
    {
	go = 1;
	
	wipe_scr = screens[0];
	(*wipes[wipeno*3])(width, height, ticks);
    }

    // do a piece of wipe-in
    V_MarkRect(0, 0, width, height);
    rc = (*wipes[wipeno*3+1])(width, height, ticks);
    //  V_DrawBlock(x, y, 0, width, height, wipe_scr); // DEBUG

    // final stuff
    if (rc)
    {
	go = 0;
	(*wipes[wipeno*3+2])(width, height, ticks);
    }

    return !go;

}
