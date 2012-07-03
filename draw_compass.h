// *********************************************************************************
// *****	draw_compass_1.h											    ********
// *****	Christopher Crews	-	21 July 2011							********
// *********************************************************************************

#ifndef _DRAWCOMPASS1FCTN
#define _DRAWCOMPASS1FCTN


// *********************************************************************************
// *****	Include Statments	 											********
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include <math.h>
#include "program_constants.h"
#include <stdio.h>
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Constants			 											********
#define COMPASS_CENTER_X		240		// x location of compass CENTER on main display
#define COMPASS_CENTER_Y		260		// y location of compass CENTER on main display
#define RADIUS					(double)(170)		// distance from center fo compass to outer edge of circle
#define	COMPASS_WIDTH			500		// width of bitmap
#define COMPASS_HEIGHT			500		// height of bitmap
#define COMPASS_VISIBLE_HEIGHT	235		// set how far down to cut off bottom of compass
#define COMPASS_VISIBLE_WIDTH	((RADIUS*2) + 4)
#define BITMAP_CENTER			(COMPASS_WIDTH/2)
#define COMPASS_X				(COMPASS_CENTER_X - (RADIUS+2))
#define COMPASS_Y				(COMPASS_CENTER_Y - (RADIUS+2))

#define HEAD_ARROW_X_OFFSET		22		// distance x to center of red dot
#define HEAD_ARROW_Y_OFFSET		200		// distance y to center of red dot
#define HEAD_ARROW_X			(COMPASS_CENTER_X - HEAD_ARROW_X_OFFSET) 
#define HEAD_ARROW_Y			(COMPASS_CENTER_Y - HEAD_ARROW_Y_OFFSET)

#define BEAR_LINE_W				(double)(6)		// width of bearing line divided by 2
#define N						4		// # of points in bearing line polygon

#define ZOOM					1		// use in rotate function for no zoom
#define SMOOTHING_ON			1		// use in rotate function to turn on smoothing
#define SMOOTHING_OFF			0		// use in rotate function to turn off smoothing

// defined round() function b/c it is not defined in the gumstix's version of math.h
#define round(x) (x<0?ceil((x)-0.5):floor((x)+0.5))
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Function Prototypes 											********

// ***** Init_Draw_Compass *****
// (r) void
// (1) struct: Pointer to data stucture
void Init_Draw_Compass(void);

// ***** Save_GPS_SDL_Surface(SDL_Surface *) *****
// (r) void
// (1) SDL_Surface *; Initialized main display surface
// (2) int: Current heading
// (3) int: Current bearing
// (4) int: Background color
void Draw_Compass(SDL_Surface *p_DisplayS, int v_heading, int v_bearing, int v_color);

// ***** Erase_Compass *****
// (r) void
// (1) SDL_Surface*: surface to erase
// (2) int: color
void Erase_Compass(SDL_Surface *p_DisplayS, int v_color);

// ----------  END  ----------------------------------------------------------------


#endif  /* _DRAWCOMPASS1FCTN  */
