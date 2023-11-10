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
//	Archiving: SaveGame I/O.
//	Thinker, Ticker.
//

#include "z_zone.h"
#include "p_local.h"

#include "doomstat.h"
#include "i_system.h"
#include "m_misc.h"
#include "p_setup.h"

fixed_t_union	leveltime;
int16_t currentThinkerListHead;
//
// THINKERS
// All thinkers should be allocated by Z_Malloc
// so they can be operated on uniformly.
// The actual structures will vary in size,
// but the first element must be thinker_t.
//



// Both the head and tail of the thinker list.
thinker_t	thinkerlist[MAX_THINKERS];



//
// P_InitThinkers
//
void P_InitThinkers (void)
{
	int16_t i;
	thinkerlist[0].next = 1;
	thinkerlist[0].prev = 1;

	
	for (i = 0; i < MAX_THINKERS; i++) {
		thinkerlist[i].prev = MAX_THINKERS;
	}

	currentThinkerListHead = 0;

}


THINKERREF P_GetNextThinkerRef(void) {

	int16_t i;
    
    for (i = currentThinkerListHead + 1; i != currentThinkerListHead; i++){
        if (i == MAX_THINKERS){
            i = 0;
        }
        
        if (thinkerlist[i].prev == MAX_THINKERS){
			currentThinkerListHead = i;
            return i;
        }

    }

#ifdef CHECK_FOR_ERRORS
	// error case
    printf("P_GetNextThinkerRef: Couldn't find a free index!");
    I_Error ("P_GetNextThinkerRef: Couldn't find a free index!");
#endif

    return -1;
    

}

int16_t addCount = 0;
//
// P_AddThinker
// Adds a new thinker at the end of the list.
//
THINKERREF P_AddThinker (MEMREF argref, THINKFUNCTION thinkfunc)
{
	// get next index
	// sets nexts, prevs
	int16_t index = P_GetNextThinkerRef();

	thinkerlist[index].next = 0;
	thinkerlist[index].prev = thinkerlist[0].prev;

	thinkerlist[thinkerlist[0].prev].next = index;
	thinkerlist[0].prev = index;

    thinkerlist[index].memref = argref;
	thinkerlist[index].functionType = thinkfunc;
	addCount++;
	return index;

}

void P_UpdateThinkerFunc(THINKERREF thinker, THINKFUNCTION argfunc) {
	thinkerlist[thinker].functionType = argfunc;
}


//
// P_RemoveThinker
// Deallocation is lazy -- it will not actually be freed
// until its thinking turn comes up.
//
void P_RemoveThinker (THINKERREF thinkerRef)
{
  // FIXME: NOP.

	thinkerlist[thinkerRef].functionType = TF_DELETEME;
}

 

int setval = 0;
//
// P_RunThinkers
//
void P_RunThinkers (void)
{
    THINKERREF	currentthinker;
	int16_t i = 0;
#ifdef DEBUGLOG_TO_FILE

	int8_t result2[100];
	int32_t lasttick = 0;
	FILE* fp;
	ticcount_t stoptic = 2500;
#endif

	currentthinker = thinkerlist[0].next;



    while (currentthinker != 0) {
		if ( thinkerlist[currentthinker].functionType == TF_DELETEME ) {
			// time to remove it
			thinkerlist[thinkerlist[currentthinker].next].prev = thinkerlist[currentthinker].prev;
			thinkerlist[thinkerlist[currentthinker].prev].next = thinkerlist[currentthinker].next;
			Z_FreeThinker (thinkerlist[currentthinker].memref);
			thinkerlist[currentthinker].prev = MAX_THINKERS;

		} else {
		

			
			
		
			if (thinkerlist[currentthinker].functionType) {

				switch (thinkerlist[currentthinker].functionType) {
					case TF_MOBJTHINKER:
						P_MobjThinker(thinkerlist[currentthinker].memref);
						break;
					case TF_PLATRAISE:
						T_PlatRaise(thinkerlist[currentthinker].memref);
						break;
					case TF_MOVECEILING:
						T_MoveCeiling(thinkerlist[currentthinker].memref);
						break;
					case TF_VERTICALDOOR:
						T_VerticalDoor(thinkerlist[currentthinker].memref);
						break;
					case TF_MOVEFLOOR:
						T_MoveFloor(thinkerlist[currentthinker].memref);
						break;
					case TF_FIREFLICKER:
						T_FireFlicker(thinkerlist[currentthinker].memref);
						break;
					case TF_LIGHTFLASH:
						T_LightFlash(thinkerlist[currentthinker].memref);
						break;
					case TF_STROBEFLASH:
						T_StrobeFlash(thinkerlist[currentthinker].memref);
						break;
					case TF_GLOW:
						T_Glow(thinkerlist[currentthinker].memref);
						break;
#ifdef CHECK_FOR_ERRORS
					default:
						I_Error("Bad thinker func! %i %i", currentthinker, thinkerlist[currentthinker].functionType);
						break;
#endif				
			

				}
#ifdef DEBUGLOG_TO_FILE
				/*
				if (gametic == 205) {
					SAVEDUNIT = Z_LoadThinkerBytesFromEMS(PLAYER_MOBJ_REF);
					if (SAVEDUNIT->momx == -43471L) {
						// i == 208: -76958L 
						I_Error("player momx momy %li %li %i", SAVEDUNIT->momx, SAVEDUNIT->momy, i);
					}
				}
				*/
				if (gametic == stoptic) {
					
					//SAVEDUNIT = Z_LoadThinkerBytesFromEMS(PLAYER_MOBJ_REF);
					if (i == 0) {
						fp = fopen("debgtick.txt", "w"); // clear old file
					} else {
						fp = fopen("debgtick.txt", "a");
					}

					fprintf(fp, "%li %hhu %i %i %hhu \n", gametic, prndindex, i, thinkerlist[currentthinker].memref, thinkerlist[currentthinker].functionType);
					fclose(fp);


				}
#endif

// i will need this later to help me debug inevitible doom 2 content memleaks
/*
				if (gametic == 619 && i == 0) {
					//SAVEDUNIT = (mobj_t*)Z_LoadThinkerBytesFromEMS(players.moRef);
					//I_Error("error %i %i %i %i %i %i %i", gametic, i, prndindex, SAVEDUNIT->x, SAVEDUNIT->y, SAVEDUNIT->momx, SAVEDUNIT->momy);
					// 454 122 157


				}
				 

				*/
				 

				i++;
			}

		}
		currentthinker = thinkerlist[currentthinker].next;
    }
#ifdef DEBUGLOG_TO_FILE
	if (gametic == stoptic) {
		I_Error("done");
	}
#endif
}



//
// P_Ticker
//

void P_Ticker (void)
{
    // run the tic
	// pause if in menu and at least one tic has been run
	if (paused || (menuactive && !demoplayback && player.viewz != 1)) {
		return;
    }
	P_PlayerThink();
	
	P_RunThinkers ();

	P_UpdateSpecials ();

	// for par times
    leveltime.w++;	
}
