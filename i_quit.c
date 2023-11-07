#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <graph.h>
#include <malloc.h>
#include "d_main.h"
#include "doomstat.h"
#include "doomdef.h"
#include "r_local.h"
#include "sounds.h"
#include "i_system.h"
#include "i_sound.h"
#include "g_game.h"
#include "m_misc.h"
#include "v_video.h"
#include "w_wad.h"
#include "z_zone.h"
#include "dmx.h"


extern union REGS regs;

//
// I_ShutdownSound
// Shuts down all sound stuff
//
void I_ShutdownSound(void)
{
	/*
	ticcount_t s;
	extern volatile ticcount_t ticcount;
	S_PauseSound();
	s = ticcount + 30;
	while (s != ticcount);
	DMX_DeInit();
	*/
}

//
// I_ShutdownGraphics
//
void I_ShutdownGraphics(void)
{
	if (*(byte *)0x449 == 0x13) // don't reset mode if it didn't get set
	{
		regs.w.ax = 3;
#ifndef	SKIP_DRAW
		intx86(0x10, &regs, &regs); // back to text mode
		//I_Error("shutdown successful");
#endif
	}
}

extern void(__interrupt __far *OldInt8)(void);
extern void (__interrupt __far *oldkeyboardisr) ();
#define KEYBOARDINT 9

extern task HeadTask;
extern int8_t TS_Installed;
extern volatile int32_t TS_InInterrupt;
#define TRUE (1 == 1)
#define FALSE (!TRUE)


void TS_FreeTaskList(void);
void TS_SetClockSpeed(int32_t speed);
uint16_t TS_SetTimer(int32_t TickBase);
void TS_SetTimerToMaxTaskRate(void);
void __interrupt __far TS_ServiceSchedule(void);
void __interrupt __far TS_ServiceScheduleIntEnabled(void);
void TS_AddTask(task *ptr);
void TS_Startup(void);
void RestoreRealTimeClock(void);



extern boolean mousepresent;
extern boolean usemouse;
int32_t I_ResetMouse(void);


/*---------------------------------------------------------------------
   Function: TS_Terminate

   Ends processing of a specific task.
---------------------------------------------------------------------*/

void TS_Terminate()

{
	_disable();
	free(&HeadTask);
	TS_SetTimerToMaxTaskRate();
	_enable();


}

/*---------------------------------------------------------------------
   Function: TS_Shutdown

   Ends processing of all tasks.
---------------------------------------------------------------------*/

void TS_Shutdown(
	void)

{
	if (TS_Installed)
	{
		TS_FreeTaskList();
		TS_SetClockSpeed(0);
		_dos_setvect(0x08, OldInt8);


		// Set Date and Time from CMOS
		//      RestoreRealTimeClock();

		TS_Installed = FALSE;
	}
}

void I_ShutdownTimer(void)
{
	TS_Terminate();
	TS_Shutdown();
}



void I_ShutdownKeyboard(void)
{
	if (oldkeyboardisr)
		_dos_setvect(KEYBOARDINT, oldkeyboardisr);
	*(int16_t *)0x41c = *(int16_t *)0x41a;      // clear bios key buffer
}
 

//
// ShutdownMouse
//
void I_ShutdownMouse(void)
{
	if (!mousepresent)
	{
		return;
	}

	I_ResetMouse();
}



#ifdef _M_I86

static int16_t emshandle;

#else
#endif


void Z_ShutdownEMS() {


#ifdef _M_I86
	int16_t result;

	if (emshandle) {

		regs.w.dx = emshandle; // handle
		regs.h.ah = 0x45;
		intx86(EMS_INT, &regs, &regs);
		result = regs.h.ah;
		if (result != 0) {
			printf("Failed deallocating EMS memory! %i!\n", result);
		}
	}
#endif

	//free(conventionalmemoryblock1);
	//free(conventionalmemoryblock2);

}



//
// I_Shutdown
// return to default system state
//
void I_Shutdown(void)
{
	I_ShutdownGraphics();
	I_ShutdownSound();
	I_ShutdownTimer();
	I_ShutdownMouse();
	I_ShutdownKeyboard();
	Z_ShutdownEMS();
}


//
// I_Quit
//
// Shuts down net game, saves defaults, prints the exit text message,
// goes to text mode, and exits.
//
void I_Quit(void)
{
	byte *scr;
	MEMREF scrRef;
	if (demorecording)
	{
		G_CheckDemoStatus();
	}

	M_SaveDefaults();
	scrRef = W_CacheLumpNameEMS("ENDOOM", PU_CACHE);
	I_ShutdownGraphics();
	I_ShutdownSound();
	I_ShutdownTimer();
	I_ShutdownMouse();
	I_ShutdownKeyboard();

	scr = Z_LoadBytesFromEMS(scrRef);
	memcpy((void *)0xb8000, scr, 80 * 25 * 2);
	regs.w.ax = 0x0200;
	regs.h.bh = 0;
	regs.h.dl = 0;
	regs.h.dh = 23;
	intx86(0x10, (union REGS *)&regs, &regs); // Set text pos

	printf("\n");
	Z_ShutdownEMS();


	exit(0);
}
