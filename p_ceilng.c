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
// DESCRIPTION:  Ceiling aninmation (lowering, crushing, raising)
//


#include "z_zone.h"
#include "doomdef.h"
#include "p_local.h"

#include "s_sound.h"

// State.
#include "doomstat.h"
#include "r_state.h"

// Data.
#include "sounds.h"

//
// CEILINGS
//


THINKERREF	activeceilings[MAXCEILINGS];


//
// T_MoveCeiling
//

void T_MoveCeiling(ceiling_t* ceiling, THINKERREF ceilingRef)
{
    result_e	res;
	sector_t* ceilingsector = &sectors[ceiling->secnum];
	sector_physics_t* ceilingsector_physics = &sectors_physics[ceiling->secnum];
	//int16_t ceilingsecnum;

    switch(ceiling->direction) {
		case 0:
			// IN STASIS
			break;
		case 1:
			// UP
			res = T_MovePlane(ceilingsector, ceiling->speed, ceiling->topheight, false,1,ceiling->direction);

			if (!(leveltime.h.fracbits &7)) {
				switch(ceiling->type) {
					case silentCrushAndRaise:
						break;
					default:
						S_StartSoundWithParams(ceilingsector_physics->soundorgX, ceilingsector_physics->soundorgY, sfx_stnmov);
						// ? 
						break;
				}
			}
			
			if (res == floor_pastdest)
			{
				switch(ceiling->type) {
					case raiseToHighest:
						P_RemoveActiveCeiling(ceiling, ceilingRef);
					break;
					
					case silentCrushAndRaise:
						S_StartSoundWithParams(ceilingsector_physics->soundorgX, ceilingsector_physics->soundorgY, sfx_pstop);
					case fastCrushAndRaise:
					case crushAndRaise:
						ceiling->direction = -1;
						break;
					
					default:
						break;
				}
				
			}
			break;
			
		case -1:
			// DOWN
			res = T_MovePlane(ceilingsector,
					ceiling->speed,
					ceiling->bottomheight,
					ceiling->crush,1,ceiling->direction);
			if (!(leveltime.h.fracbits &7))
			{
				switch(ceiling->type) {
					case silentCrushAndRaise: break;
						default:
						S_StartSoundWithParams(ceilingsector_physics->soundorgX, ceilingsector_physics->soundorgY, sfx_stnmov);
				}
			}
			
			if (res == floor_pastdest)
			{
				switch(ceiling->type) {
					case silentCrushAndRaise:
						S_StartSoundWithParams(ceilingsector_physics->soundorgX, ceilingsector_physics->soundorgY, sfx_pstop);
					case crushAndRaise:
						ceiling->speed = CEILSPEED;
					case fastCrushAndRaise:
						ceiling->direction = 1;
						break;

					case lowerAndCrush:
					case lowerToFloor:
						P_RemoveActiveCeiling(ceiling, ceilingRef);
						break;

					default:
						break;
				}
			} else { // ( res != floor_pastdest )
				if (res == floor_crushed) {
					switch(ceiling->type) {
						case silentCrushAndRaise:
						case crushAndRaise:
						case lowerAndCrush:
							ceiling->speed = CEILSPEED / 8;
							break;

						default:
							break;
					}
				}
			}
		break;
    }
}


//
// EV_DoCeiling
// Move a ceiling up/down and all around!
//
int16_t
EV_DoCeiling
( uint8_t linetag,
  ceiling_e	type )
{
    int16_t		secnum;
    int16_t		rtn;
    sector_t*	sector;
    sector_physics_t*	sector_physics;
	int16_t		j = 0;
    ceiling_t*	ceiling;
	THINKERREF ceilingRef;
	int16_t secnumlist[MAX_ADJOINING_SECTORS];

    secnum = -1;
    rtn = 0;
    
    //	Reactivate in-stasis ceilings...for certain types.
    switch(type) {
      case fastCrushAndRaise:
      case silentCrushAndRaise:
      case crushAndRaise:
		P_ActivateInStasisCeiling(linetag);
	default:
		break;
    }
	
	P_FindSectorsFromLineTag(linetag, secnumlist, false);

	while (secnumlist[j] >= 0) {
		secnum = secnumlist[j];
		j++;
			

		sector = &sectors[secnum];
		sector_physics = &sectors_physics[secnum];

		// new door thinker
		rtn = 1;

		ceiling = (ceiling_t*) P_CreateThinker (TF_MOVECEILING_HIGHBITS);
		ceiling->secnum = secnum;
		ceiling->crush = false;
		ceilingRef = GETTHINKERREF(ceiling);

		sectors[secnum].specialdataRef = ceilingRef;

		switch(type)
		{
		  case fastCrushAndRaise:
			ceiling->crush = true;
			ceiling->topheight = sector->ceilingheight;
			ceiling->bottomheight = sector->floorheight+(8 << SHORTFLOORBITS);
			ceiling->direction = -1;
			ceiling->speed = CEILSPEED * 2;
			break;

		  case silentCrushAndRaise:
		  case crushAndRaise:
			ceiling->crush = true;
			ceiling->topheight = sector->ceilingheight;
		  case lowerAndCrush:
		  case lowerToFloor:
			ceiling->bottomheight = sector->floorheight;
			if (type != lowerToFloor)
				ceiling->bottomheight += (8 << SHORTFLOORBITS);
			ceiling->direction = -1;
			ceiling->speed = CEILSPEED;
			break;

		  case raiseToHighest: {
			  short_height_t ceilingtopheight = P_FindHighestCeilingSurrounding(secnum);
			  ceiling->topheight = ceilingtopheight;
			  ceiling->direction = 1;
			  ceiling->speed = CEILSPEED;
			  break;
			   }
		}
		
		ceiling->tag = sector_physics->tag;
		ceiling->type = type;
		P_AddActiveCeiling(ceilingRef);
    }
    return rtn;
}


//
// Add an active ceiling
//
void P_AddActiveCeiling(THINKERREF ceilingRef) {
    int8_t		i;
    
    for (i = 0; i < MAXCEILINGS;i++) {
		if (activeceilings[i] == NULL_THINKERREF) {
			activeceilings[i] = ceilingRef;
			return;
		}
    }
}



//
// Remove a ceiling's thinker
//
void P_RemoveActiveCeiling(ceiling_t* c, THINKERREF ceilingRef)
{
    int8_t		i;

    for (i = 0;i < MAXCEILINGS;i++) {
		if (activeceilings[i] == ceilingRef) {
			sectors[c->secnum].specialdataRef = NULL_THINKERREF;
			P_RemoveThinker (ceilingRef);
			activeceilings[i] = NULL_THINKERREF;
			break;
		}
    }
}



//
// Restart a ceiling that's in-stasis
//
void P_ActivateInStasisCeiling(uint8_t linetag)
{
    int8_t		i;
	ceiling_t* c;

	for (i = 0; i < MAXCEILINGS; i++) {
		if (activeceilings[i] != NULL_THINKERREF) {
			c = (ceiling_t*)&thinkerlist[activeceilings[i]].data;
			if ((c->tag == linetag) && (c->direction == 0)) {
				c->direction = c->olddirection;

				P_UpdateThinkerFunc(activeceilings[i], TF_MOVECEILING_HIGHBITS);
			}
		}
	}
}



//
// EV_CeilingCrushStop
// Stop a ceiling from crushing!
//
int16_t	EV_CeilingCrushStop(uint8_t linetag)
{
    int8_t		i;
    int16_t		rtn;
	ceiling_t* c;
	rtn = 0;
	for (i = 0; i < MAXCEILINGS; i++) {
		if (activeceilings[i] != NULL_THINKERREF) {
			c = (ceiling_t*)&thinkerlist[activeceilings[i]].data;
			if ((c->tag == linetag) && (c->direction != 0)) {
				c->olddirection = c->direction;
				P_UpdateThinkerFunc(activeceilings[i], TF_NULL);
				c->direction = 0;		// in-stasis
				rtn = 1;
			}
		}
	}

    return rtn;
}
