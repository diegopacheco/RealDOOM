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
//	The not so system specific sound interface.
//


#ifndef __S_SOUND__
#define __S_SOUND__

#include "z_zone.h"
#include "doomdef.h"

//
// Initializes sound stuff, including volume
// Sets channels, SFX and music volume,
//  allocates channel buffer, sets S_sfx lookup.
//
void
S_Init
( int16_t		sfxVolume,
  int16_t		musicVolume );




//
// Per level startup code.
// Kills playing sounds at start of level,
//  determines music if any, changes music.
//
void S_Start(void);

void S_StartSoundFromRef(MEMREF memref, int32_t	sfx_id);


//
// Start sound for thing at <origin>
//  using <sound_id> from sounds.h
//
void
S_StartSound
(void*		origin,
  int32_t		sound_id );

void
S_StartSoundWithParams(int32_t x, int32_t y, int32_t sound_id);



// Will start a sound at a given volume.
void
S_StartSoundAtVolume
( MEMREF		originRef,
	fixed_t x,
	fixed_t y,
  int32_t		sound_id,
  int32_t		volume );


// Stop sound for thing at <origin>
void S_StopSound(MEMREF originRef);


// Start music using <music_id> from sounds.h
void S_StartMusic(int32_t music_id);

// Start music using <music_id> from sounds.h,
//  and set whether looping
void
S_ChangeMusic
( int32_t		music_id,
  int32_t		looping );

// Stops the music fer sure.
void S_StopMusic(void);

// Stop and resume music, during game PAUSE.
void S_PauseSound(void);
void S_ResumeSound(void);


//
// Updates music & sounds
//
void S_UpdateSounds(MEMREF listenerRef);

void S_SetMusicVolume(uint8_t volume);
void S_SetSfxVolume(uint8_t volume);


#endif
