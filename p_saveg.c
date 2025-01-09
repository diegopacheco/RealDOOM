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
//

#include "i_system.h"
#include "z_zone.h"
#include "p_local.h"

// State.
#include "doomstat.h"
#include "r_state.h"
#include "m_near.h"


// Pads save_p to a 4-byte boundary
//  so that the load/save works on SGI&Gecko.
#define PADSAVEP()	save_p += (4 - ((int32_t) save_p & 3)) & 3



//
// P_ArchivePlayers
//
void P_ArchivePlayers (void) {
		
	
	PADSAVEP();

	FAR_memcpy (save_p,&player,sizeof(player_t));
	save_p += sizeof(player_t);
	FAR_memcpy (save_p,psprites,NUMPSPRITES*sizeof(pspdef_t));
	save_p += NUMPSPRITES*sizeof(player_t);
//	for (j=0 ; j<NUMPSPRITES ; j++) {
//		if (psprites[j].state) {
//			psprites[j].state  = (state_t *)(psprites[j].state-states);
//		}
//	}
	
}



//
// P_UnArchivePlayers
//
void P_UnArchivePlayers (void) {
	
	
	PADSAVEP();

	FAR_memcpy (&player,save_p, sizeof(player_t));
	save_p += sizeof(player_t);
	
	// will be set when unarc thinker
	playerMobjRef = NULL_THINKERREF;	
	player.message = -1;
	player.attackerRef = NULL_THINKERREF;

	FAR_memcpy (psprites,save_p, NUMPSPRITES*sizeof(pspdef_t));
	save_p += NUMPSPRITES*sizeof(pspdef_t);

	
}


//
// P_ArchiveWorld
//
void P_ArchiveWorld (void) {
	
    int16_t			i;
    int16_t			j;
    sector_t 		 __far*		sec;
    sector_physics_t __far*		sec_phys;

    line_t			 __far*		li;
    line_physics_t	 __far*		li_phys;


    side_t  		__far*		si;
    side_render_t  	__far*		si_rend;
    int16_t 		__far*      put = (int16_t __far*)save_p;
    
    // do sectors




    for (i=0, sec = sectors, sec_phys = sectors_physics ;  i<numsectors ; i++,sec++,sec_phys++) {
		*put++ = sec->floorheight >> SHORTFLOORBITS;
		*put++ = sec->ceilingheight >> SHORTFLOORBITS;
		*put++ = sec->floorpic;
		*put++ = sec->ceilingpic;
		*put++ = sec->lightlevel;
		*put++ = sec_phys->special;		
		*put++ = sec_phys->tag;		
    }

	
	// todo page in si_render

    // do lines
    for (i=0, li = lines, li_phys = lines_physics ; i<numlines ; i++,li++,li_phys++) {
		
		*put++ = lineflagslist[i]; // todo bit 9?
		*put++ = li_phys->special;
		*put++ = li_phys->tag;
		for (j=0 ; j<2 ; j++) {
			if (li->sidenum[j] == -1){
				continue;
			}
			
			si 		= &sides[li->sidenum[j]];
			si_rend = &sides_render[li->sidenum[j]];
			*put++ 	= si->textureoffset;
			*put++ 	= si_rend->rowoffset;
			*put++ 	= si->toptexture;
			*put++ 	= si->bottomtexture;
			*put++ 	= si->midtexture;	
		}
    }
	
    save_p = (byte *)put;
	
}



//
// P_UnArchiveWorld
//
void P_UnArchiveWorld (void) {
	
    int16_t			i;
    int16_t			j;
    sector_t 			__far*		sec;
    sector_physics_t 	__far*		sec_phys;
    line_t  			__far*		li;
    line_physics_t  	__far*		li_phys;
    side_t  			__far*		si;
    side_render_t  		__far*		si_rend;
    int16_t 			__far*		get = (int16_t __far*)save_p;

    // do sectors
    for (i=0, sec = sectors, sec_phys = sectors_physics; i<numsectors ; i++,sec++,sec_phys++) {
		sec->floorheight   	= *get++ << SHORTFLOORBITS;
		sec->ceilingheight 	= *get++ << SHORTFLOORBITS;
		sec->floorpic      	= *get++;
		sec->ceilingpic    	= *get++;
		sec->lightlevel 	= *get++;
		sec_phys->special 		= *get++;		// needed?
		sec_phys->tag 			= *get++;		// needed?
		sec_phys->specialdataRef = NULL_THINKERREF;
		//sec_phys->soundtargetRef = NULL_THINKERREF;
    }
    
    // do lines
	for (i=0 ; i<numlines ; i++,li++) {
		li = &lines[i];
		li_phys = &lines_physics[i];
		lineflagslist[i] 	= *get++;  // todo bit 9?
		li_phys->special 		= *get++;
		li_phys->tag 			= *get++;
		for (j=0 ; j<2 ; j++) {
			if (li->sidenum[j] == -1){
				continue;
			}

			si 					= &sides[li->sidenum[j]];
			si_rend 			= &sides_render[li->sidenum[j]];
			si->textureoffset 	= *get++;
			si_rend->rowoffset  = *get++;
			si->toptexture 		= *get++;
			si->bottomtexture 	= *get++;
			si->midtexture 		= *get++;
		}
    }
    save_p = (byte *)get;	
	
}





//
// Thinkers
//
typedef enum {
    tc_end,
    tc_mobj

} thinkerclass_t;



//
// P_ArchiveThinkers
//
void P_ArchiveThinkers (void) {
	
    THINKERREF				th;
	mobj_t __near*		mobj;
	
    // save off the current thinkers
    for (th = thinkerlist[0].next ; th != 0; th=thinkerlist[th].next) {
		if (thinkerlist[th].prevFunctype & TF_FUNCBITS == TF_MOBJTHINKER_HIGHBITS) {
			mobj = &thinkerlist[th].data;

			*save_p++ = tc_mobj;
			PADSAVEP();
			FAR_memcpy (save_p, mobj, sizeof(mobj_t));
			save_p += sizeof(mobj_t);
			//mobj->state = (state_t *)(mobj->state - states);
			
			// todo what to do here
			//if (mobj->player)
				//mobj->player = mobj->player
			continue;
		}
		
		//I_Error ("P_ArchiveThinkers: Unknown thinker function");
    }

    // add a terminating marker
    *save_p++ = tc_end;	
	
	}


void  __near P_InitThinkers (void);

//
// P_UnArchiveThinkers
//
void P_UnArchiveThinkers (void) {
	
    byte				tclass;
    THINKERREF			currentthinker;
	THINKERREF			next;
	THINKERREF 			thinkerRef;
	mobj_t __near* 		mobj;
	mobj_pos_t __far * 	mobj_pos;
	
    
    // remove all the current thinkers
    currentthinker = thinkerlist[0].next;
	while (currentthinker != 0) {
		next = thinkerlist[currentthinker].next;

		if (thinkerlist[currentthinker].prevFunctype & TF_FUNCBITS == TF_MOBJTHINKER_HIGHBITS) {
			P_RemoveMobj(&thinkerlist[currentthinker].data);
		} else {
			memset(&thinkerlist[currentthinker].data, 0, sizeof(mobj_t));
		}

		currentthinker = next;
    }
    P_InitThinkers ();
	
    // read in saved thinkers
    while (1) {
		tclass = *save_p++;
		switch (tclass) {
			case tc_end:
				return; 	// end of list
					
			case tc_mobj:
				PADSAVEP();
				
				mobj =  P_CreateThinker(TF_MOBJTHINKER_HIGHBITS);
				thinkerRef = GETTHINKERREF(mobj);
				mobj_pos = &mobjposlist[thinkerRef];
				FAR_memcpy (mobj, save_p, sizeof(mobj_t));
				save_p += sizeof(mobj_t);
				//mobj->state = &states[(int16_t)mobj->state];
				mobj->targetRef = NULL_THINKERREF;
				
				// todo player detect
				/*
				if (mobj->player) {
					mobj->player = &players;
					mobj->player->moRef = thinkerRef;
				}
				*/
				P_SetThingPosition (mobj, mobj_pos, mobj->secnum);
				//mobj->info = &mobjinfo[mobj->type];
				mobj->floorz = sectors[mobj->secnum].floorheight;
				mobj->ceilingz = sectors[mobj->secnum].ceilingheight;

				//mobj->thinkerRef = P_AddThinker (thinkerRef, TF_MOBJTHINKER);
				break;
				
					
			default:
				I_Error ("Unknown tclass %i in savegame",tclass);
		}
	
    }
	
}


//
// P_ArchiveSpecials
//

/*
enum {
    tc_ceiling,
    tc_door,
    tc_floor,
    tc_plat,
    tc_flash,
    tc_strobe,
    tc_glow,
    tc_endspecials

} specials_e;	
*/


//
// Things to handle:
//
// T_MoveCeiling, (ceiling_t: sector_t * swizzle), - active list
// T_VerticalDoor, (vldoor_t: sector_t * swizzle),
// T_MoveFloor, (floormove_t: sector_t * swizzle),
// T_LightFlash, (lightflash_t: sector_t * swizzle),
// T_StrobeFlash, (strobe_t: sector_t *),
// T_Glow, (glow_t: sector_t *),
// T_PlatRaise, (plat_t: sector_t *), - active list
//
void P_ArchiveSpecials (void) {
	/*
    THINKERREF		th;
    ceiling_t*		ceiling;
    vldoor_t*		door;
    floormove_t*	floor;
    plat_t*		plat;
    lightflash_t*	flash;
    strobe_t*		strobe;
    glow_t*		glow;
    int16_t			i;
	void*		thinkerobj;
	
    // save off the current thinkers
    for (th = thinkerlist[0].next ; th != 0 ; th=thinkerlist[th].next) {
		thinkerobj = Z_LoadThinkerFromConventional(thinkerlist[th].memref);
		if (thinkerlist[th].functionType == TF_NULL) {
			for (i = 0; i < MAXCEILINGS;i++)
				if (activeceilings[i] == thinkerlist[th].memref)
					break;
	    
			if (i<MAXCEILINGS) {
				*save_p++ = tc_ceiling;
				PADSAVEP();
				ceiling = (ceiling_t *)save_p;
				FAR_memcpy (ceiling, thinkerobj, sizeof(*ceiling));
				save_p += sizeof(*ceiling);
				//ceiling->secnum = ceiling->secnum
			}
			continue;
		}
			
		if (thinkerlist[th].functionType == TF_MOVECEILING) {
			*save_p++ = tc_ceiling;
			PADSAVEP();
			ceiling = (ceiling_t *)save_p;
			FAR_memcpy (ceiling, thinkerobj, sizeof(*ceiling));
			save_p += sizeof(*ceiling);
			//ceiling->secnum = ceiling->secnum
			
			continue;
		}
			
		if (thinkerlist[th].functionType == TF_VERTICALDOOR) {
			*save_p++ = tc_door;
			PADSAVEP();
			door = (vldoor_t *)save_p;
			FAR_memcpy (door, thinkerobj, sizeof(*door));
			save_p += sizeof(*door);
			//door->secnum = door->secnum;
			continue;
		}
			
		if (thinkerlist[th].functionType == TF_MOVEFLOOR) {
			*save_p++ = tc_floor;
			PADSAVEP();
			floor = (floormove_t *)save_p;
			FAR_memcpy (floor, thinkerobj, sizeof(*floor));
			save_p += sizeof(*floor);
			//floor->secnum = floor->secnum;
			continue;
		}
			
		if (thinkerlist[th].functionType == TF_PLATRAISE) {
			*save_p++ = tc_plat;
			PADSAVEP();
			plat = (plat_t *)save_p;
			FAR_memcpy (plat, thinkerobj, sizeof(*plat));
			save_p += sizeof(*plat);
			//plat->secnum = plat->secnum;
			continue;
		}
			
		if (thinkerlist[th].functionType == TF_LIGHTFLASH) {
			*save_p++ = tc_flash;
			PADSAVEP();
			flash = (lightflash_t *)save_p;
			FAR_memcpy (flash, thinkerobj, sizeof(*flash));
			save_p += sizeof(*flash);
			//flash->secnum = flash->secnum;
			continue;
		}
			
		if (thinkerlist[th].functionType == TF_STROBEFLASH) {
			*save_p++ = tc_strobe;
			PADSAVEP();
			strobe = (strobe_t *)save_p;
			FAR_memcpy (strobe, thinkerobj, sizeof(*strobe));
			save_p += sizeof(*strobe);
			//strobe->secnum = strobe->secnum;
			continue;
		}
			
		if (thinkerlist[th].functionType == TF_GLOW) {
			*save_p++ = tc_glow;
			PADSAVEP();
			glow = (glow_t *)save_p;
			FAR_memcpy (glow, thinkerobj, sizeof(*glow));
			save_p += sizeof(*glow);
			//glow->secnum = glow->secnum;
			continue;
		}
    }
	


    // add a terminating marker
    *save_p++ = tc_endspecials;	
	*/
}


//
// P_UnArchiveSpecials
//
void P_UnArchiveSpecials (void) {
	/*
    byte		tclass;
    ceiling_t*		ceiling;
    vldoor_t*		door;
    floormove_t*	floor;
    plat_t*		plat;
    lightflash_t*	flash;
    strobe_t*		strobe;
    glow_t*		glow;
	THINKERREF thinkerRef;
	
    // read in saved thinkers
    /*
	while (1)
    {
	tclass = *save_p++;
	function.acp1 = NULL;
	switch (tclass)
	{
	  case tc_endspecials:
	    return;	// end of list
			
	  case tc_ceiling:
	    PADSAVEP();
		thinkerRef = Z_MallocEMS(sizeof(*ceiling), PU_LEVEL, 0xFF, ALLOC_TYPE_LEVSPEC);
		ceiling = (ceiling_t*)Z_LoadBytesFromEMS(thinkerRef);

	    FAR_memcpy (ceiling, save_p, sizeof(*ceiling));
	    save_p += sizeof(*ceiling);
	    ceiling->sector = &sectors[(int16_t)ceiling->sector];
	    ceiling->sector->specialdataRef = thinkerRef;

		if (ceiling->thinkerRef.function.acp1) {
			function.acp1 = (actionf_p1)T_MoveCeiling;
		}
 

		function.acp1 = (actionf_p1)T_MoveFloor;
		ceiling->thinkerRef = P_AddThinker(thinkerRef, function);


	    P_AddActiveCeiling(ceiling);
	    break;
				
	  case tc_door:
	    PADSAVEP();
		thinkerRef = Z_MallocEMS(sizeof(*door), PU_LEVEL, 0xFF, ALLOC_TYPE_LEVSPEC);
		door = (vldoor_t*)Z_LoadBytesFromEMS(thinkerRef);

	    FAR_memcpy (door, save_p, sizeof(*door));
	    save_p += sizeof(*door);
	    door->sector = &sectors[(int16_t)door->sector];
	    door->sector->specialdataRef = thinkerRef;
	    door->thinker.function.acp1 = (actionf_p1)T_VerticalDoor;
		door->thinker.memref = thinkerRef;
		P_AddThinker (&door->thinker);
	    break;
				
	  case tc_floor:
	    PADSAVEP();
		thinkerRef = Z_MallocEMS(sizeof(*floor), PU_LEVEL, 0xFF, ALLOC_TYPE_LEVSPEC);
		floor = (floormove_t*)Z_LoadBytesFromEMS(thinkerRef);

	    FAR_memcpy (floor, save_p, sizeof(*floor));
	    save_p += sizeof(*floor);
	    floor->sector = &sectors[(int16_t)floor->sector];
	    floor->sector->specialdataRef = thinkerRef;
	    floor->thinker.function.acp1 = (actionf_p1)T_MoveFloor;
		floor->thinker.memref = thinkerRef;
		P_AddThinker (&floor->thinker);
	    break;
				
	  case tc_plat:
	    PADSAVEP();
		thinkerRef = Z_MallocEMS(sizeof(*plat), PU_LEVEL, 0xFF, ALLOC_TYPE_LEVSPEC);
		plat = (plat_t*)Z_LoadBytesFromEMS(thinkerRef);

	    FAR_memcpy (plat, save_p, sizeof(*plat));
	    save_p += sizeof(*plat);
	    plat->sector = &sectors[(int16_t)plat->sector];
	    plat->sector->specialdataRef = thinkerRef;

	    if (plat->thinker.function.acp1)
		plat->thinker.function.acp1 = (actionf_p1)T_PlatRaise;

		plat->thinker.memref = thinkerRef;
		P_AddThinker (&plat->thinker);
	    P_AddActivePlat(thinkerRef);
	    break;
				
	  case tc_flash:
	    PADSAVEP();
		thinkerRef = Z_MallocEMS(sizeof(*flash), PU_LEVEL, 0xFF, ALLOC_TYPE_LEVSPEC);
		flash = (lightflash_t*)Z_LoadBytesFromEMS(thinkerRef);

	    FAR_memcpy (flash, save_p, sizeof(*flash));
	    save_p += sizeof(*flash);
	    flash->sector = &sectors[(int16_t)flash->sector];
	    flash->thinker.function.acp1 = (actionf_p1)T_LightFlash;
		flash->thinker.memref = thinkerRef;
		P_AddThinker (&flash->thinker);
	    break;
				
	  case tc_strobe:
	    PADSAVEP();
		thinkerRef = Z_MallocEMS(sizeof(*strobe), PU_LEVEL, 0xFF, ALLOC_TYPE_LEVSPEC);
		strobe = (strobe_t*)Z_LoadBytesFromEMS(thinkerRef);
		
	    FAR_memcpy (strobe, save_p, sizeof(*strobe));
	    save_p += sizeof(*strobe);
	    strobe->sector = &sectors[(int16_t)strobe->sector];
	    strobe->thinker.function.acp1 = (actionf_p1)T_StrobeFlash;
		strobe->thinker.memref = thinkerRef;
		break;
				
	  case tc_glow:
	    PADSAVEP();
		thinkerRef = Z_MallocEMS(sizeof(*glow), PU_LEVEL, 0xFF, ALLOC_TYPE_LEVSPEC);
		glow = (glow_t*)Z_LoadBytesFromEMS(thinkerRef);

	    FAR_memcpy (glow, save_p, sizeof(*glow));
	    save_p += sizeof(*glow);
	    glow->sector = &sectors[(int16_t)glow->sector];
	    glow->thinker.function.acp1 = (actionf_p1)T_Glow;
		glow->thinker.memref = thinkerRef;
		P_AddThinker (&glow->thinker);
	    break;
				
	  default:
	    I_Error ("P_UnarchiveSpecials:Unknown tclass %i "
		     "in savegame",tclass);
	}
	
    }*/

}

