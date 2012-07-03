// *********************************************************************************
// *****	draw_all_screens.h												********
// *****	Richard Manley	-	05 January 2012								********
// *********************************************************************************

#ifndef _DRAWALLSCREENSFCTN
#define _DRAWALLSCREENSFCTN


// *********************************************************************************
// *****	Include Statments	 											********
#include "program_constants.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <math.h>
#include "draw_functions.h"
#include "gps.h"
#include "compass.h"
#include "draw_Lat.h"
#include "draw_Long.h"
#include "draw_compass.h"
#include "draw_time.h"
#include "draw_fix.h"
#include "main.h"
#include "draw_distance.h"

// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Function Prototypes 											********

// ***** Draw_Nav_Screen *****
// (r) int;  0 = succesfull 1 = error
// (1) SDL_Surface *; initalized Display Surface
int Draw_Nav_Screen(SDL_Surface*,int, int, int);

// ----------  END  ----------------------------------------------------------------


#endif  /* _DRAWALLSCREENSFCTN  */
