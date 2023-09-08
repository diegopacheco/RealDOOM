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
//	Main loop menu stuff.
//	Default Config File.
//

#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <ctype.h>


#include "doomdef.h"

#include "z_zone.h"

#include "w_wad.h"

#include "i_system.h"
#include "v_video.h"

#include "hu_stuff.h"

// State.
#include "doomstat.h"

// Data.
#include "dstrings.h"

#include "m_misc.h"


int16_t		myargc;
int8_t**		myargv;

//
// M_DrawText
// Returns the final X coordinate
// HU_Init must have been called to init the font
//
extern MEMREF		hu_fontRef[HU_FONTSIZE];

 

//
// M_CheckParm
// Checks for the given parameter
// in the program's command line arguments.
// Returns the argument number (1 to argc-1)
// or 0 if not present
int16_t M_CheckParm (int8_t *check)
{
    int16_t		i;

    for (i = 1;i<myargc;i++)
    {
	if ( !strcasecmp(check, myargv[i]) )
	    return i;
    }

    return 0;
}


//
// M_Random
// Returns a 0-255 number
//
uint8_t rndtable[256] = {
    0,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66 ,
    74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36 ,
    95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188 ,
    52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224 ,
    149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242 ,
    145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   0 ,
    175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235 ,
    25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113 ,
    94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75 ,
    136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196 ,
    135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113 ,
    80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241 ,
    24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224 ,
    145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95 ,
    28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226 ,
    71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36 ,
    17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106 ,
    197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136 ,
    120, 163, 236, 249
};

int16_t	rndindex = 0;
int16_t	prndindex = 0;

// Which one is deterministic?
uint8_t P_Random(void)
{
	 
    prndindex = (prndindex+1)&0xff;
    return rndtable[prndindex];
}

uint8_t M_Random (void)
{
    rndindex = (rndindex+1)&0xff;
    return rndtable[rndindex];
}

void M_ClearRandom (void)
{
    rndindex = prndindex = 0;
}
 

void
M_AddToBox16
( int16_t*	box,
  int16_t	x,
  int16_t	y )
{
    if (x<box[BOXLEFT])
	box[BOXLEFT] = x;
    else if (x>box[BOXRIGHT])
	box[BOXRIGHT] = x;
    if (y<box[BOXBOTTOM])
	box[BOXBOTTOM] = y;
    else if (y>box[BOXTOP])
	box[BOXTOP] = y;
}
 
void M_ClearBox16(int16_t *box)
{
	box[BOXTOP] = box[BOXRIGHT] = MINSHORT;
	box[BOXBOTTOM] = box[BOXLEFT] = MAXSHORT;
}
 
//
// M_WriteFile
//
#ifndef O_BINARY
#define O_BINARY 0
#endif

boolean
M_WriteFile
(int8_t const*	name,
  void*		source,
  filelength_t		length )
{
    filelength_t		handle;
    filelength_t		count;
	
    handle = open ( name, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0666);

    if (handle == -1)
	return false;

    count = write (handle, source, length);
    close (handle);
	
    if (count < length)
	return false;
		
    return true;
}


//
// M_ReadFile
//
filelength_t
M_ReadFile
(int8_t const*	name,
  MEMREF*	bufferRef )
{
    filelength_t	handle, count, length;
    struct stat	fileinfo;
    byte		*buf;
	
    handle = open (name, O_RDONLY | O_BINARY, 0666);
#ifdef CHECK_FOR_ERRORS

	if (handle == -1)
		I_Error ("Couldn't read file %s", name);
#endif
	if (fstat (handle,&fileinfo) == -1)
		I_Error ("Couldn't read file %s", name);
    length = fileinfo.st_size;
    *bufferRef = Z_MallocEMSNew (length, PU_STATIC, 0xff, ALLOC_TYPE_READFILE);
	buf = Z_LoadBytesFromEMS(*bufferRef);
    count = read (handle, buf, length);
    close (handle);
#ifdef CHECK_FOR_ERRORS
    if (count < length)
		I_Error ("Couldn't read file %s", name);
#endif		
    //*buffer = buf;
    return length;
}


//
// DEFAULTS
//
uint8_t		usemouse;

extern uint8_t	key_right;
extern uint8_t	key_left;
extern uint8_t	key_up;
extern uint8_t	key_down;

extern uint8_t	key_strafeleft;
extern uint8_t	key_straferight;

extern uint8_t	key_fire;
extern uint8_t	key_use;
extern uint8_t	key_strafe;
extern uint8_t	key_speed;

extern uint8_t	mousebfire;
extern uint8_t	mousebstrafe;
extern uint8_t	mousebforward;

extern int16_t	viewwidth;
extern int16_t	viewheight;

extern uint8_t	mouseSensitivity;
extern uint8_t	showMessages;

extern uint8_t	detailLevel;

extern uint8_t	screenblocks;


// machine-independent sound params
extern	uint8_t	numChannels;

extern uint8_t sfxVolume;
extern uint8_t musicVolume;
extern uint8_t snd_SBport8bit, snd_SBirq, snd_SBdma;
extern uint8_t snd_Mport8bit;



typedef struct
{
    int8_t*	name;
    uint8_t*	location;
    uint8_t		defaultvalue;
    uint8_t		scantranslate;		// PC scan code hack
    uint8_t		untranslated;		// lousy hack
} default_t;

#define SC_UPARROW              0x48
#define SC_DOWNARROW            0x50
#define SC_LEFTARROW            0x4b
#define SC_RIGHTARROW           0x4d
#define SC_RCTRL                0x1d
#define SC_RALT                 0x38
#define SC_RSHIFT               0x36
#define SC_SPACE                0x39
#define SC_COMMA                0x33
#define SC_PERIOD               0x34
#define SC_PAGEUP               0x49
#define SC_INSERT               0x52
#define SC_HOME                 0x47
#define SC_PAGEDOWN             0x51
#define SC_DELETE               0x53
#define SC_END                  0x4f
#define SC_ENTER                0x1c

#define SC_KEY_A                0x1e
#define SC_KEY_B                0x30
#define SC_KEY_C                0x2e
#define SC_KEY_D                0x20
#define SC_KEY_E                0x12
#define SC_KEY_F                0x21
#define SC_KEY_G                0x22
#define SC_KEY_H                0x23
#define SC_KEY_I                0x17
#define SC_KEY_J                0x24
#define SC_KEY_K                0x25
#define SC_KEY_L                0x26
#define SC_KEY_M                0x32
#define SC_KEY_N                0x31
#define SC_KEY_O                0x18
#define SC_KEY_P                0x19
#define SC_KEY_Q                0x10
#define SC_KEY_R                0x13
#define SC_KEY_S                0x1f
#define SC_KEY_T                0x14
#define SC_KEY_U                0x16
#define SC_KEY_V                0x2f
#define SC_KEY_W                0x11
#define SC_KEY_X                0x2d
#define SC_KEY_Y                0x15
#define SC_KEY_Z                0x2c
#define SC_BACKSPACE            0x0e

default_t	defaults[] =
{
    {"mouse_sensitivity",&mouseSensitivity, 5},
    {"sfx_volume",&sfxVolume, 8},
    {"music_volume",&musicVolume, 8},
    {"show_messages",&showMessages, 1},
    
    {"key_right",&key_right, SC_RIGHTARROW, 1},
    {"key_left",&key_left, SC_LEFTARROW, 1},
    {"key_up",&key_up, SC_UPARROW, 1},
    {"key_down",&key_down, SC_DOWNARROW, 1},
    {"key_strafeleft",&key_strafeleft, SC_COMMA, 1},
    {"key_straferight",&key_straferight, SC_PERIOD, 1},

    {"key_fire",&key_fire, SC_RCTRL, 1},
    {"key_use",&key_use, SC_SPACE, 1},
    {"key_strafe",&key_strafe, SC_RALT, 1},
    {"key_speed",&key_speed, SC_RSHIFT, 1},

    {"use_mouse",&usemouse, 1},
    {"mouseb_fire",&mousebfire,0},
    {"mouseb_strafe",&mousebstrafe,1},
    {"mouseb_forward",&mousebforward,2},

    {"screenblocks",&screenblocks, 9},
    {"detaillevel",&detailLevel, 0},

    {"snd_channels",&numChannels, 3},
    {"snd_musicdevice",&snd_DesiredMusicDevice, 0},
    {"snd_sfxdevice",&snd_DesiredSfxDevice, 0},
    {"snd_sbport",&snd_SBport8bit, 0x22}, // must be shifted one...
    {"snd_sbirq",&snd_SBirq, 5},
    {"snd_sbdma",&snd_SBdma, 1},
    {"snd_mport",&snd_Mport8bit, 0x33},  // must be shifted one..

    {"usegamma",&usegamma, 0}
	 

};

int8_t	numdefaults;
int8_t*	defaultfile;


//
// M_SaveDefaults
//
void M_SaveDefaults (void)
{
    int8_t		i;
    int8_t		v;
    FILE*	f;
	
    f = fopen (defaultfile, "w");
    if (!f)
	    return; // can't write the file, but don't complain
		
    for (i=0 ; i<numdefaults ; i++) {
        if (defaults[i].scantranslate){
            defaults[i].location = &defaults[i].untranslated;
        }
        //if (defaults[i].defaultvalue > -0xfff && defaults[i].defaultvalue < 0xfff) {
            v = *defaults[i].location;
            fprintf (f,"%s\t\t%i\n",defaults[i].name,v);
        //} else {
        //    fprintf (f,"%s\t\t\"%s\"\n",defaults[i].name,
        //        * (int8_t **) (defaults[i].location));
        //}
    }
	
    fclose (f);
}


//
// M_LoadDefaults
//
extern byte	scantokey[128];

void M_LoadDefaults (void)
{
    int16_t		i;
    filelength_t		len;
    FILE*	f;
	int8_t	def[80];
	int8_t	strparm[100];
    int8_t*	newstring;
    uint8_t		parm;
    boolean	isstring;
    
    // set everything to base values
    numdefaults = sizeof(defaults)/sizeof(defaults[0]);
    for (i=0 ; i<numdefaults ; i++)
	*defaults[i].location = defaults[i].defaultvalue;
    
    // check for a custom default file
    i = M_CheckParm ("-config");
    if (i && i<myargc-1) {
        defaultfile = myargv[i+1];
        printf ("	default file: %s\n",defaultfile);
    } else {
	    defaultfile = basedefault;
    }
    // read the file in, overriding any set defaults
    f = fopen (defaultfile, "r");
    if (f) {
        while (!feof(f)) {
            isstring = false;
            if (fscanf (f, "%79s %[^\n]\n", def, strparm) == 2) {
                if (strparm[0] == '"') {
                    // get a string default
                    isstring = true;
                    len = strlen(strparm);
                    newstring = (int8_t *) malloc(len);
                    strparm[len-1] = 0;
                    strcpy(newstring, strparm+1);
                } else if (strparm[0] == '0' && strparm[1] == 'x'){
                    sscanf(strparm+2, "%x", &parm);
                } else {
                    sscanf(strparm, "%i", &parm);
                }
                for (i=0 ; i<numdefaults ; i++){
                    if (!strcmp(def, defaults[i].name)) {
                        if (!isstring){
                            *defaults[i].location = parm;
                        } else {
                            *defaults[i].location = (uint8_t)newstring;
                        }
                        break;
                    }
                }
            }
        }
            
        fclose (f);
    }
    for (i = 0; i < numdefaults; i++)
    {
        if (defaults[i].scantranslate)
        {
            parm = *defaults[i].location;
            defaults[i].untranslated = parm;
            *defaults[i].location = scantokey[parm];
        }
    }
}



