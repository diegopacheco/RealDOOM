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
//      Zone Memory Allocation. Neat.
//

#include "z_zone.h"
#include "i_system.h"
#include "doomdef.h"

#include "m_menu.h"
#include "w_wad.h"
#include "r_data.h"

#include "doomstat.h"
#include "r_bsp.h"
#include "r_local.h"
#include "p_local.h"
#include "v_video.h"
#include "st_stuff.h"
#include "hu_stuff.h"
#include "wi_stuff.h"

#include <dos.h>

#include <stdlib.h>
#include "memory.h"





extern union REGS regs;
extern struct SREGS segregs;
 
uint16_t EMS_PAGE;
// EMS STUFF
extern int16_t emshandle;


byte __far* __near Z_InitEMS()
{

	// 4 mb
	// todo test 3, 2 MB, etc. i know we use less..
	int16_t numPagesToAllocate = 256; //  (4 * 1024 * 1024) / PAGE_FRAME_SIZE;
	int16_t pageframebase;


	// todo check for device...
	// char	emmname[9] = "EMMXXXX0";



	int16_t pagestotal, pagesavail;
	int16_t errorreg;
	uint8_t vernum;
	int16_t j;
	DEBUG_PRINT("  Checking EMS...");



	regs.h.ah = 0x40;
	int86(EMS_INT, &regs, &regs);
	errorreg = regs.h.ah;
	if (errorreg) {
		I_Error("91 %d", errorreg); // Couldn't init EMS, error %d
	}


	regs.h.ah = 0x46;
	intx86(EMS_INT, &regs, &regs);
	vernum = regs.h.al;
	errorreg = regs.h.ah;
	if (errorreg != 0) {
		I_Error("90"); // EMS Error 0x46
	}
	//DEBUG_PRINT("Version %i", vernum);
	if (vernum < 40) {
		I_Error("Expected EMS 4.0, found %x", vernum);
	}

	// get page frame address
	regs.h.ah = 0x41;
	intx86(EMS_INT, &regs, &regs);
	pageframebase = regs.w.bx;
	errorreg = regs.h.ah;
	if (errorreg != 0) {
		I_Error("89");/// EMS Error 0x41
	}




	regs.h.ah = 0x42;
	intx86(EMS_INT, &regs, &regs);
	pagesavail = regs.w.bx;
	pagestotal = regs.w.dx;
	DEBUG_PRINT("\n  %i pages total, %i pages available at frame %p", pagestotal, pagesavail, pageframebase);

	if (pagesavail < NUM_EMS4_SWAP_PAGES) {
		I_Error("\nERROR: minimum of %i EMS pages required", NUM_EMS4_SWAP_PAGES);
	}


	regs.w.bx = numPagesToAllocate;
	regs.h.ah = 0x43;
	intx86(EMS_INT, &regs, &regs);
	emshandle = regs.w.dx;
	errorreg = regs.h.ah;
	if (errorreg != 0) {
		// Error 0 = 0x00 = no error
		// Error 137 = 0x89 = zero pages
		// Error 136 = 0x88 = OUT_OF_LOG
		I_Error("88 %i", errorreg);// EMS Error 0x43
	}


	// do initial page remapping


	for (j = 0; j < 4; j++) {
		regs.h.al = j;  // physical page
		regs.w.bx = j;    // logical page
		regs.w.dx = emshandle; // handle
		regs.h.ah = 0x44;
		intx86(EMS_INT, &regs, &regs);
		if (regs.h.ah != 0) {
			I_Error("87"); // EMS Error 0x44
		}
	}


	//*size = numPagesToAllocate * PAGE_FRAME_SIZE;

	// EMS Handle
	EMS_PAGE = pageframebase;
	return  MK_FP(pageframebase, 0);




}

 
 

extern int16_t pagenum9000;
extern int16_t pageswapargs[total_pages];
extern int16_t pageswapargoff;

  



void __near Z_GetEMSPageMap() {
	int16_t pagedata[256]; // i dont think it can get this big...
	int16_t __far* pointervalue = pagedata;
	int16_t errorreg, i, numentries;
 

	regs.w.ax = 0x5801;  // physical page
	intx86(EMS_INT, &regs, &regs);
	errorreg = regs.h.ah;
	numentries = regs.w.cx;
	if (errorreg != 0) {
		I_Error("84 %i", errorreg);// \nCall 5801 failed with value %i!\n
	}
	DEBUG_PRINT("\n Found: %i mappable EMS pages (28+ required)", numentries);

	regs.w.ax = 0x5800;  // physical page
	segregs.es = (uint16_t)((uint32_t)pointervalue >> 16);
	regs.w.di = (uint16_t)(((uint32_t)pointervalue) & 0xffff);
	intx86(EMS_INT, &regs, &regs);
	errorreg = regs.h.ah;
	//pagedata = MK_FP(sregs.es, regs.w.di);
	if (errorreg != 0) {
		I_Error("83 %i", errorreg);// \nCall 25 failed with value %i!\n
	}
 
	for (i = 0; i < numentries; i++) {
		if (pagedata[i * 2] == 0x9000u) {
			pagenum9000 = pagedata[(i * 2) + 1];
			goto found;
		}
	}

	//I_Error("\nMappable page for segment 0x9000 NOT FOUND! EMS 4.0 features unsupported?\n");

found:

	// cache these args
	//pageswapargseg = (uint16_t)((uint32_t)pageswapargs >> 16);
	pageswapargoff = (uint16_t)(((uint32_t)pageswapargs) & 0xffff);
	 

	

	//					PHYSICS			RENDER					ST/HUD			DEMO		PALETTE			FWIPE				MENU		INTERMISSION
	// BLOCK
	// --------------------------------------------------------------------------------------------------------------------------------------------------
	// UMB BLOCK		
	// (0xE000)			level data
	// --------------------------------------------------------------------------------------------------------------------------------------------------
	// UMB HALF-BLOCK
	// 0xc800			sprite data
	// --------------------------------------------------------------------------------------------------------------------------------------------------
	//					lumpinfo		textures			screen4 0x9c00
	// 0x9000 block		empty											palettebytes	fwipe temp data					screen1
	// --------------------------------------------------------------------------------------------------------------------------------------------------
	//									tex cache arrays
	// 									sprite stuff			
	//					screen0			visplane openings									screen0			screen 0						screen0
	// 0x8000 block		gamma table		texture memrefs?									gamma table		gamma table		
	// --------------------------------------------------------------------------------------------------------------------------------------------------
	// 0x7000 block		physics levdata render levdata			st graphics									screen 2		menu graphics	 
	//                                  flat cache?
	// --------------------------------------------------------------------------------------------------------------------------------------------------
	//					more physics levdata zlight																screen 3
	//                  rejectmatrix
	// 					nightnmarespawns textureinfo																		menu graphics	menu graphics
	// 0x6000 block		strings									strings															strings			strings
	// --------------------------------------------------------------------------------------------------------------------------------------------------
	//									flat cache
	//					events			events
	//                  states          states																[scratch buffer]				[scratch used
	// 0x5000 block		trig tables   	trig tables								demobuffer													for anims]
	// --------------------------------------------------------------------------------------------------------------------------------------------------
	//                  some common vars visplane stuff
	// 0x4000 block		thinkers		viewangles, drawsegs
	// --------------------------------------------------------------------------------------------------------------------------------------------------
	// NON EMS STUFF BELOW - ALWAYS MAPPED 
	// --------------------------------------------------------------------------------------------------------------------------------------------------
	// 0x3c00           (eventually) DS
	// 0x3200           sine/cosine/trig tables
	// 0x3000 block		
	// --------------------------------------------------------------------------------------------------------------------------------------------------


	for (i = 1; i < total_pages; i+= 2) {
		pageswapargs[i] += pagenum9000;
	}
	 

	Z_QuickMapLumpInfo5000();

	FAR_memcpy((byte __far *) 0x54000000, (byte __far *) 0x94000000, 49152u); // copy the wad lump stuff over. gross
	FAR_memset((byte __far *) 0x94000000, 0, 49152u);

	Z_QuickMapPhysics(); // map default page map
}


//extern byte __far* pageFrameArea;
extern int16_t emshandle;


	void __far R_DrawColumn (void);
	void __far R_DrawColumnLow(void);

void PSetupEndFunc();
void __far P_SetupLevel (int8_t episode, int8_t map, skill_t skill);
boolean __far P_CheckSight (  mobj_t __far* t1, mobj_t __far* t2, mobj_pos_t __far* t1_pos, mobj_pos_t __far* t2_pos );
 
void __near Z_LoadBinaries() {
	int i;
	FILE* fp;
	// currently in physics region!
	fp = fopen("D_INFO.BIN", "rb"); 
	FAR_fread(InfoFuncLoadAddr, 1, SIZE_D_INFO, fp);
	fclose(fp);

	// load R_DrawColumn into high memory near colormaps...
	FAR_memcpy(colfunc_function_area,
	(byte __far *)R_DrawColumn, 
	(byte __far *)R_DrawColumnLow - (byte __far *)R_DrawColumn);

	#ifdef MOVE_P_SIGHT
		FAR_memcpy(PSightFuncLoadAddr, (byte __far *)P_CheckSight, SIZE_PSight);
	#endif
	
	#ifdef MOVE_P_SETUP
		FAR_memcpy(PSetupFuncLoadAddr, (byte __far *)P_SetupLevel, SIZE_PSetup);
	#endif
	// copy psetup and pfunc..
	// 6736 bytes
	//FAR_memcpy(PSetupFuncLoadAddr, (byte __far *)P_SetupLevel, 0x1A50);


	// all data now in this file instead of spread out a
	fp = fopen("DOOMDATA.BIN", "rb"); 
	
	//256
	FAR_fread(rndtable, 1, 256, fp);
	//128
	FAR_fread(scantokey, 1, 128, fp);
	
	//1507
	FAR_fread(mobjinfo, sizeof(mobjinfo_t), NUMMOBJTYPES, fp);
	DEBUG_PRINT(".");

	//5802
	FAR_fread(states, sizeof(state_t), NUMSTATES, fp);
	DEBUG_PRINT(".");

	//1280
	FAR_fread(gammatable, 1, 5 * 256, fp);
	DEBUG_PRINT(".");

	//40960
	FAR_fread(finesine, 4, 10240, fp);
	DEBUG_PRINT(".");

	//8192
	FAR_fread(finetangentinner, 4, 2048, fp);
	DEBUG_PRINT(".");

	//274
	FAR_fread(doomednum, 2, NUMMOBJTYPES, fp);

	// just load them all here in one call instead of 5 like below
	Z_QuickMapIntermission();
	//760
	FAR_fread(lnodex, 1, 760, fp);

	/*
	FAR_fread(lnodex, 2, 9 * 3, fp);
	FAR_fread(lnodey, 2, 9 * 3, fp);
	FAR_fread(epsd0animinfo, 16, 10, fp);
	FAR_fread(epsd1animinfo, 16, 9, fp);
	FAR_fread(epsd2animinfo, 16, 6, fp);
	FAR_fread(wigraphics, 1, 28 * 9, fp);
	*/
	
	

	//I_Error("\n%i %i %i %i", epsd1animinfo[2].period, epsd1animinfo[2].loc.x, anims[1][2].period, anims[1][2].loc.x);
	Z_QuickMapRender();
 
	//4096
	FAR_fread(zlight, 1, 4096, fp);
	FAR_fread(fuzzoffset, 1, 50, fp);

	Z_QuickMapPhysics();
	FAR_fread(pars, 2, 72, fp);  // 4*10 + 32 par times

	fclose(fp);

	//I_Error("\n%x %x %x %x", lnodex[0], lnodex[1], lnodex[2], lnodex[3]);

	DEBUG_PRINT("..");


	//fp = fopen("D_TANTOA.BIN", "rb");
	//FAR_fread(tantoangle, 4, 2049, fp);
	//fclose(fp);
	 
}



