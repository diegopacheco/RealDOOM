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
//	Plats (i.e. elevator platforms) code, raising/lowering.
//


#include "i_system.h"
#include "z_zone.h"
#include "m_misc.h"

#include "doomdef.h"
#include "p_local.h"

#include "s_sound.h"

// State.
#include "doomstat.h"
#include "r_state.h"

// Data.
#include "sounds.h"


MEMREF		activeplats[MAXPLATS];



//
// Move a plat up and down
//
void T_PlatRaise(MEMREF platRef)
{

    result_e	res;
	plat_t* plat = (plat_t*)Z_LoadBytesFromEMS(platRef);
	short platsecnum = plat->secnum;

	sector_t* sectors = (sector_t*)Z_LoadBytesFromEMS(sectorsRef);
	int sectorsoundorgX = sectors[platsecnum].soundorgX;
	int sectorsoundorgY = sectors[platsecnum].soundorgY;
	fixed_t sectorfloorheight = sectors[platsecnum].floorheight;

	plat = (plat_t*)Z_LoadBytesFromEMS(platRef);



	switch(plat->status) {
		  case up:
				res = T_MovePlane(plat->secnum,
						  plat->speed,
						  plat->high,
						  plat->crush,0,1);
				plat = (plat_t*)Z_LoadBytesFromEMS(platRef);
				if (plat->type == raiseAndChange || plat->type == raiseToNearestAndChange) {
					if (!(leveltime & 7)) {
						S_StartSoundWithParams(sectorsoundorgX, sectorsoundorgY, sfx_stnmov);
					}
				}
	
				
				if (res == crushed && (!plat->crush)) {
					plat->count = plat->wait;
					plat->status = down;
					S_StartSoundWithParams(sectorsoundorgX, sectorsoundorgY, sfx_pstart);
				} else {
					if (res == pastdest) {
						plat->count = plat->wait;
						plat->status = waiting;
						S_StartSoundWithParams(sectorsoundorgX, sectorsoundorgY, sfx_pstop);

						switch(plat->type) {
						  case blazeDWUS:
						  case downWaitUpStay:
							P_RemoveActivePlat(platRef);
							break;
		    
						  case raiseAndChange:
						  case raiseToNearestAndChange:
							P_RemoveActivePlat(platRef);
							break;
		    
						  default:
							break;
						}
					}
				}
				break;
	
		  case	down:
				res = T_MovePlane(platsecnum,plat->speed,plat->low,false,0,-1);
				plat = (plat_t*)Z_LoadBytesFromEMS(platRef);
				if (res == pastdest) {
					plat->count = plat->wait;
					plat->status = waiting;
					S_StartSoundWithParams(sectorsoundorgX, sectorsoundorgY, sfx_pstop);
				}
				break;
	
		  case	waiting:
			  if (!--plat->count) {
					if (sectorfloorheight == plat->low)
						plat->status = up;
					else
						plat->status = down;
					S_StartSoundWithParams(sectorsoundorgX, sectorsoundorgY, sfx_pstart);
			  }
		  case	in_stasis:
			  break;
    }
}


//
// Do Platforms
//  "amount" is only used for SOME platforms.
//
int
EV_DoPlat
(  short linetag,
	short lineside0,
  plattype_e	type,
  int		amount )
{
    plat_t*	plat;
    int		secnum;
    int		rtn;
	MEMREF platRef;
	side_t* sides;
	short side0secnum;
	fixed_t specialheight;
	int sectorsoundorgX;
	int sectorsoundorgY;
	fixed_t sectorfloorheight;
	sector_t* sectors;

    secnum = -1;
    rtn = 0;

    //	Activate all <type> plats that are in_stasis
    switch(type) {
		  case perpetualRaise:
			P_ActivateInStasis(linetag);
			break;
	
		  default:
			break;
	}
	
	sides = (side_t*)Z_LoadBytesFromEMS(sidesRef);
	side0secnum = sides[lineside0].secnum;
	while ((secnum = P_FindSectorFromLineTag(linetag,secnum)) >= 0) {
		sectors = (sector_t*)Z_LoadBytesFromEMS(sectorsRef);
 


		if ((&sectors[secnum])->specialdataRef) {
			continue;
		}
		// Find lowest & highest floors around sector
		rtn = 1;


		sectorsoundorgX = sectors[secnum].soundorgX;
		sectorsoundorgY = sectors[secnum].soundorgY;
		sectorfloorheight = sectors[secnum].floorheight;
		platRef = Z_MallocEMSNew(sizeof(*plat), PU_LEVSPEC, 0, ALLOC_TYPE_LEVSPEC);
		(&sectors[secnum])->specialdataRef = platRef;
		plat = (plat_t*)Z_LoadBytesFromEMS(platRef);
		plat->thinkerRef = P_AddThinker(platRef, TF_PLATRAISE);
	 

		plat->type = type;
		plat->secnum = secnum;
		plat->crush = false;
		plat->tag = linetag;

		 

		switch (type) {
			case raiseToNearestAndChange:
				plat->speed = PLATSPEED / 2;
				sectors = (sector_t*)Z_LoadBytesFromEMS(sectorsRef);
				(&sectors[secnum])->floorpic = sectors[side0secnum].floorpic;
				specialheight = P_FindNextHighestFloor(secnum, sectorfloorheight);
				plat = (plat_t*)Z_LoadBytesFromEMS(platRef);
				plat->high = specialheight;
				plat->wait = 0;
				plat->status = up;
				// NO MORE DAMAGE, IF APPLICABLE
				sectors = (sector_t*)Z_LoadBytesFromEMS(sectorsRef);
				(&sectors[secnum])->special = 0;

				S_StartSoundWithParams(sectorsoundorgX, sectorsoundorgY, sfx_stnmov);
				break;

			case raiseAndChange:
				sectors = (sector_t*)Z_LoadBytesFromEMS(sectorsRef);
				(&sectors[secnum])->floorpic = sectors[side0secnum].floorpic;

				plat = (plat_t*)Z_LoadBytesFromEMS(platRef);
				plat->speed = PLATSPEED / 2;
				plat->high = sectorfloorheight + amount * FRACUNIT;
				plat->wait = 0;
				plat->status = up;

				S_StartSoundWithParams(sectorsoundorgX, sectorsoundorgY, sfx_stnmov);
				break;

			case downWaitUpStay:
				plat->speed = PLATSPEED * 4;
				specialheight = P_FindLowestFloorSurrounding(secnum);
				plat = (plat_t*)Z_LoadBytesFromEMS(platRef);
				plat->low = specialheight;

				if (plat->low > sectorfloorheight) {
					plat->low = sectorfloorheight;
				}
				plat->high = sectorfloorheight;
				plat->wait = 35 * PLATWAIT;
				plat->status = down;

				S_StartSoundWithParams(sectorsoundorgX, sectorsoundorgY, sfx_pstart);
				break;

			case blazeDWUS:
				plat->speed = PLATSPEED * 8;
				specialheight = P_FindLowestFloorSurrounding(secnum);
				plat = (plat_t*)Z_LoadBytesFromEMS(platRef);

				plat->low = specialheight;

				if (plat->low > sectorfloorheight) {
					plat->low = sectorfloorheight;
				}
				plat->high = sectorfloorheight;
				plat->wait = 35 * PLATWAIT;
				plat->status = down;
				S_StartSoundWithParams(sectorsoundorgX, sectorsoundorgY, sfx_pstart);
				break;

			case perpetualRaise:
				plat->speed = PLATSPEED;
				specialheight = P_FindLowestFloorSurrounding(secnum);
				if (specialheight > sectorfloorheight) {
					specialheight = sectorfloorheight;
				}

				plat = (plat_t*)Z_LoadBytesFromEMS(platRef);
				plat->low = specialheight;

				specialheight = P_FindHighestFloorSurrounding(secnum);
				plat = (plat_t*)Z_LoadBytesFromEMS(platRef);
				plat->high = specialheight;

				if (plat->high < sectorfloorheight) {
					plat->high = sectorfloorheight;
				}

				plat->wait = 35*PLATWAIT;
				plat->status = P_Random()&1;

				S_StartSoundWithParams(sectorsoundorgX, sectorsoundorgY, sfx_pstart);
				break;
		}
		P_AddActivePlat(platRef);
    }
    return rtn;
}



void P_ActivateInStasis(int tag) {
    int		j;
	plat_t* plat;
	for (j = 0; j < MAXPLATS; j++)
		if (activeplats[j] != NULL_MEMREF) {
			plat = (plat_t*)Z_LoadBytesFromEMS(activeplats[j]);
			if ((plat->status == in_stasis) && (plat->tag == tag)) {
				plat->oldstatus = plat->status;

				P_UpdateThinkerFunc(plat->thinkerRef, TF_PLATRAISE);
			}
		}

}

void EV_StopPlat(short linetag) {
	int		j;
	plat_t* plat;

	for (j = 0; j < MAXPLATS; j++) {
		if (activeplats[j] != NULL_MEMREF) {
			plat = (plat_t*)Z_LoadBytesFromEMS(activeplats[j]);
			if ((plat->status != in_stasis) && (plat->tag == linetag)) {
				plat->oldstatus = plat->status;
				plat->status = in_stasis;

				P_UpdateThinkerFunc(plat->thinkerRef, TF_NULL);
			}
		}
	}
}

static int platraisecount = 0;
static int addedplatraisecount = 0;
static int platindex = 0;

void P_AddActivePlat(MEMREF memref) {
    int		i;
	addedplatraisecount++;
    for (i = 0;i < MAXPLATS;i++)
	if (activeplats[i] == NULL_MEMREF) {
	    activeplats[i] = memref;
		platindex = memref;
	    return;
	}
    I_Error ("P_AddActivePlat: no more plats!");
}



void P_RemoveActivePlat(MEMREF platRef)
{
    int		i;
	plat_t* plat;
	sector_t* sectors;
	short platsecnum;
	THINKERREF platthinkerRef;
	platraisecount++;
	for (i = 0; i < MAXPLATS; i++) {
		if (platRef == activeplats[i]) {
			plat = (plat_t*)Z_LoadBytesFromEMS(activeplats[i]);
			platsecnum = plat->secnum;
			platthinkerRef = plat->thinkerRef;

			sectors = (sector_t*)Z_LoadBytesFromEMS(sectorsRef);
			(&sectors[platsecnum])->specialdataRef = NULL_MEMREF;

			P_RemoveThinker(platthinkerRef);
			activeplats[i] = NULL_MEMREF;

			return;
		}
	}
    I_Error ("P_RemoveActivePlat: can't find plat! %i %i %i %i", platRef, platraisecount, addedplatraisecount, platindex);
}
