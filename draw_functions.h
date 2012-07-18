// *********************************************************************************
// *****	draw_functions.h												********
// *****	Christopher Crews	-	20 Jan 2011								********
// *********************************************************************************

#ifndef _DRAWFCTN
#define _DRAWFCTN


// *********************************************************************************
// *****	Include Statments	 											********
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <math.h>
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Veriable Declarations 											********
typedef struct{
		int x1;
		int y1;
		int x2;
		int y2;
} Struct_Coordinates;
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Function Prototypes 											********

// ***** Prepair_Surface *****
// (r) void;
// (1) SDL_Surface *; initalized Display Surface
// (2) SDL_Surface *; Text Surface
// (4) SDL_Rect *; Structure holding the coordinates of the source
// (4) SDL_Rect *; Structure holding the coordinates for the destination
// (5) Struct_Coordinates *; Structure holding the old & new loacation coordinates
void Prepair_Surface(SDL_Surface*, SDL_Surface*, SDL_Rect*, SDL_Rect*, Struct_Coordinates*);

// ***** Setup_Coordinates *****
// (r) void;
// (1) SDL_Surface *; Text Surface
// (2) SDL_Rect *; Structure holding the coordinates of the source
// (3) SDL_Rect *; Structure holding the coordinates for the destination
// (4) Struct_Coordinates *; Structure holding the old & new loacation coordinates
void Setup_Coordinates(SDL_Surface*, SDL_Rect*, SDL_Rect*, Struct_Coordinates*);


// ***** Erase Data *****
// static function; private to draw_distance.c
// (r) void
// (1) SDL_Surface *; initalized Display Surface
// (2) Struct_Coordinates;  xy of left corner
// (3) SDL_Color;  background color
// (4) int;  border   0 = no boarder  1 = border
void Erase_Data(SDL_Surface*, Struct_Coordinates, SDL_Color, int);

// ----------  END  ----------------------------------------------------------------


#endif  /* _DRAWFCTN  */
