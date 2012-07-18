// *********************************************************************************
// *****	Christopher Crews 	20 Jan 2011									********
// *****	Program Name:	draw_functions.c								********
// *****	holds functions and structures common to all draw files			********
// *********************************************************************************
// *****             Formated for Monospaced Type							********


// *********************************************************************************
// *****	Include Statments	 											********
#include "draw_functions.h"
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Prepair_Surface													********
// *****	Clear the last data from the screen and set up the new coordinates******
// *****	(r) void														********
// *****	(1) SDL_Surface *; pointer to initalized display surface		********
// *****	(2) SDL_Surface *; pointer to text surface						********
// *****	(3) SDL_Rect *; pointer to a coordinate structure				********
// *****	(4) SDL_Rect *; pointe to hold the coordinats to print text		********
// *****	(3) Struct_Coordinates *; pointer to hold new and old 			********
// *****			coordinates for the text.								********
// *********************************************************************************
void Prepair_Surface(SDL_Surface* p_DS, SDL_Surface* p_TS, SDL_Rect* p_sorce, SDL_Rect* p_dest, Struct_Coordinates* s_xy)
{
// *********************************************************************************
// *****  	Fill Source Coor Data											********
p_sorce->x = 0;
p_sorce->y = 0;
p_sorce->w = p_TS->w;
p_sorce->h = p_TS->h;
p_dest->w = p_sorce->w;
p_dest->h = p_sorce->h;
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****  Set the new location to the destination structure and save old location***
p_dest->x = s_xy->x1;
p_dest->y = s_xy->y1;
// ----------  END  ----------------------------------------------------------------

return;
}
// ----------  END PREPAIR_SURFACE  ------------------------------------------------


// *********************************************************************************
// *****	Setup_Coordinates												********
// *****	Set up the new coordinates										******
// *****	(r) void														********
// *****	(2) SDL_Surface *; pointer to text surface						********
// *****	(3) SDL_Rect *; pointer to a coordinate structure				********
// *****	(4) SDL_Rect *; pointe to hold the coordinats to print text		********
// *****	(3) Struct_Coordinates *; pointer to hold new					********
// *****			coordinates for the text.								********
// *********************************************************************************
void Setup_Coordinates(SDL_Surface* p_TS, SDL_Rect* p_sorce, SDL_Rect* p_dest, Struct_Coordinates* s_xy)
{
// *********************************************************************************
// *****  	Fill Source Coor Data											********
p_sorce->x = 0;
p_sorce->y = 0;
p_sorce->w = p_TS->w;
p_sorce->h = p_TS->h;
p_dest->w = p_sorce->w;
p_dest->h = p_sorce->h;
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****  Set the new location to the destination structure and save old location***
p_dest->x = s_xy->x1;
p_dest->y = s_xy->y1;

// ----------  END  ----------------------------------------------------------------

return;
}
// ----------  END PREPAIR_SURFACE  ------------------------------------------------




// *********************************************************************************
// *****	Erase_Data														********
// *****	Erase the old data 												********
// *****	(r) void														********
// *****	(1) Struct_Coordinates *:  coordinate of left corner of box		********
// *****	(2) Struct_Coordinates *:  coordinate of left corner of box		********
// *****	(3) SDL_Color;  background color								********
// *********************************************************************************
void Erase_Data(SDL_Surface* p_DS, Struct_Coordinates s, SDL_Color v_bg, int v_border)
{
	// ***** Erase old data by covering up with box *****
boxRGBA(p_DS,  s.x1, s.y1, s.x2, s.y2, v_bg.r, v_bg.g, v_bg.b,255);	

	// ***** Box Border *****
if(v_border)
rectangleRGBA(p_DS, s.x1, s.y1, s.x2, s.y2, 0,0,255,255);
return;
}
// ----------  END Erase_Data ------------------------------------------------------

