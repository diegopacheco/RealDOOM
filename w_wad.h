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
//	WAD I/O functions.
//


#ifndef __W_WAD__
#define __W_WAD__
#include "r_defs.h"


//
// TYPES
//
typedef struct
{
    // Should be "IWAD" or "PWAD".
	int8_t		identification[4];
    int32_t			numlumps;
    int32_t			infotableofs;
    
} wadinfo_t;


typedef struct
{
    int32_t			filepos;
    int32_t			size;
	int8_t		name[8];
    
} filelump_t;

//
// WADFILE I/O related stuff.
//
typedef struct
{
	int8_t	name[8];
    int32_t		handle;
    int32_t		position;
    int32_t		size;
} lumpinfo_t;


extern	lumpinfo_t*	lumpinfo;
extern	int32_t		numlumps;

void    W_InitMultipleFiles (int8_t** filenames);
void    W_Reload (void);

int32_t	W_CheckNumForName (int8_t* name);
int32_t	W_GetNumForName (int8_t* name);

int32_t	W_LumpLength (int32_t lump);
void    W_ReadLump (int32_t lump, void *dest);

int32_t W_CacheLumpNumCheck(int32_t lump, int32_t error);
MEMREF  W_CacheLumpNumEMS(int16_t lump, int8_t tag);

//void*	W_CacheLumpName (int8_t* name, int32_t tag);
MEMREF  W_CacheLumpNameEMS(int8_t* name, int32_t tag);
void	W_EraseLumpCache(int16_t index);
patch_t* W_CacheLumpNameEMSAsPatch (int8_t*         name, int32_t           tag);



#endif
