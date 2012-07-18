// *********************************************************************************
// *****	draw_altitude.h													********
// *****	Christopher Crews	-	23 May 2011								********
// *********************************************************************************

#ifndef _DRAWLATFCTN
#define _DRAWLATFCTN


// *********************************************************************************
// *****	Include Statments	 											********
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <math.h>
#include "draw_functions.h"
#include "program_constants.h"
// ----------  END  ----------------------------------------------------------------



// *********************************************************************************
// *****	Constants			 											********
#define LAT_X				(KEYSTONE_X + 10)
#define LAT_Y				(KEYSTONE_Y + 30)
#define LAT_W				210
#define LAT_H				100
#define DATA_Y_OFFSET 		35

#define DATA_FONT_SIZE 		50
#define LABEL_FONT_SIZE 	40
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Function Prototypes 											********

// ***** Draw_Altitude *****
// (r) void
// (1) SDL_Surface *; initalized Display Surface
// (2) int; Estimated Altitude Altitude
// (3) int; Units 0 =  ft  1 = m  2 = ft
// (4) int; Screen Color   0 = black  255 = white
// (5) int; GPS fix   0 = no fix  1 = 2D Fix  3 = 3D Fix 
void Draw_Lat(SDL_Surface*,float, int);

// ----------  END  ----------------------------------------------------------------


#endif  /* _DRAWALTITUDEFCTN  */
