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
//	Rendering of moving objects, sprites.
//


#ifndef __R_THINGS__
#define __R_THINGS__


extern vissprite_t __far*	vissprite_p;
extern vissprite_t	vsprsortedhead;

// Constant arrays used for psprite clipping
//  and initializing clipping.

// vars for R_DrawMaskedColumn
extern int16_t __far*		mfloorclip;
extern int16_t __far*		mceilingclip;
extern fixed_t_union		spryscale;
extern fixed_t_union		sprtopscreen;

extern uint16_t		pspritescale;
extern fixed_t		pspriteiscale;

void __near R_DrawSingleMaskedColumn (segment_t pixeldata_segment, byte length);
void __near R_DrawMaskedColumn (segment_t pixeldata_segment, column_t __far* postdata);
void __near R_DrawMaskedSpriteShadow (segment_t pixeldata, column_t __far* column);

void __near R_SortVisSprites (void);

void __near R_AddSprites(sector_t __far* sec);
void __near R_ClearSprites (void);
void __near R_DrawMasked (void);

void __near R_PrepareMaskedPSprites(void);

#endif
