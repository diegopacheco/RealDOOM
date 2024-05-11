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
//

#ifndef __M_MISC__
#define __M_MISC__


#include "doomdef.h"
#include "doomtype.h"
#include "z_zone.h"
//
// MISC
//
extern  int16_t	myargc;
extern  int8_t**	myargv;
extern int16_t	prndindex;


// Returns the position of the given parameter
// in the arg list (0 if not found).
int16_t M_CheckParm (int8_t* check);

boolean
M_WriteFile
(int8_t const*	name,
  void __far*		source,
  filelength_t		length );

filelength_t
M_ReadFile
(int8_t const*	name,
  byte __far*	bufferRef );

// Returns a number from 0 to 255,
// from a lookup table.
uint8_t M_Random(void);

// As M_Random, but used only by the play simulation.
uint8_t P_Random(void);


// Fix randoms for demos.
void M_ClearRandom(void);

// Bounding box coordinate storage.


#define    BOXTOP 0
#define    BOXBOTTOM 1
#define    BOXLEFT 2
#define    BOXRIGHT 3
	// bbox coordinates

// Bounding box functions.
void M_AddToBox16( int16_t __near*	box,int16_t	x,  int16_t	y );


void M_LoadDefaults (void);

void M_SaveDefaults (void);

#define NUM_DEFAULTS 28


typedef struct
{
	int8_t  __near*	name;
	uint8_t __near*	location;
	uint8_t		defaultvalue;
	uint8_t		scantranslate;		// PC scan code hack
	uint8_t		untranslated;		// lousy hack
} default_t;


extern default_t	defaults[NUM_DEFAULTS];

#endif
