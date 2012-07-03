// *********************************************************************************
// *****	Richard Manley 	17 Jan 2011										********
// *****	Program Name:	draw_altitude.c									********
// *****	Takes an integer input and prints it to the screen using SDL	********
// *********************************************************************************
// *****             Formated for Monospaced Type							********


// *********************************************************************************
// *****	Include Statments	 											********
#include "draw_time.h"
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Draw_Altitude													********
// *****	Draws the Est. Altitude Altitude to the screen					********
// *****	(r) int; 0 = success 1 = error									********
// *****	(1) int; Estimated Altitude Altitude							********
// *****	(2) int; x coor													********
// *****	(3) int; y coor													********
// *********************************************************************************
void Draw_Time(SDL_Surface* p_DisplayS,int v_hours, int v_mins, int v_secs)
{
// *********************************************************************************
// ***** Veriabel Deleration 												********
const char v_lable[] = "Time";
char v_data_string[20];
TTF_Font* p_Font_Label;
TTF_Font* p_Font_Data;
SDL_Color v_color;
SDL_Color v_bgcolor;
SDL_Rect v_SrcCoor;
SDL_Rect v_DstCoor;
Struct_Coordinates s_box;
Struct_Coordinates s_data;
Struct_Coordinates s_label;

SDL_Surface* p_TextSurface;
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****  	Configure Data to be displayed									********

	if(v_secs > 60) v_secs = 0;
	sprintf(v_data_string,"%d:%02d:%d",v_hours,v_mins,v_secs);

// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****  	Setup Colors											********

	// ***** text color *****
	v_color.r = 0;
	v_color.g = 255;
	v_color.b = 0;
	// ***** background color *****	
	v_bgcolor.r = 0;
	v_bgcolor.g = 0;
	v_bgcolor.b = 0;

// ----------  END  ----------------------------------------------------------------		


// *********************************************************************************
// *****  	Opening and setting font size for all text						********
	// ***** Open and set Data Font Size *****
p_Font_Data = TTF_OpenFont(FONT_DIR"/ProFontWindows.ttf",DATA_FONT_SIZE);

	// ***** Open and set Label Font Size *****
p_Font_Label = TTF_OpenFont(FONT_DIR"/ProFontWindows.ttf",LABEL_FONT_SIZE);

	// ***** Error Checking *****
if (!p_Font_Data || !p_Font_Label)
{
	printf("Altitude TTF_OpenFont Error: %s\n", TTF_GetError());
	return;
}
// ----------  END  ----------------------------------------------------------------
 

// *********************************************************************************
// *****  	Erase old data and draw border									********
	// ***** Set Box Position *****
s_box.x1 = TIME_X;
s_box.y1 = TIME_Y;
s_box.x2 = TIME_X + TIME_W;
s_box.y2 = TIME_Y + TIME_H;
	// ***** Erase old data by covering up with box *****
Erase_Data(p_DisplayS, s_box, v_bgcolor, NO_BORDER);
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****  	Drawing Est. Altitude Altitude									********
	// ***** Set Text Position *****
s_data.x1 = TIME_X;
s_data.y1 = TIME_Y + DATA_Y_OFFSET;

	// ***** puts data into text surface *****
p_TextSurface = TTF_RenderText_Blended(p_Font_Data, v_data_string, v_color);

	// *****  Send text surface to the screen  *****
Setup_Coordinates(p_TextSurface, &v_SrcCoor, &v_DstCoor, &s_data);
SDL_BlitSurface(p_TextSurface, NULL, p_DisplayS, &v_DstCoor);
SDL_FreeSurface(p_TextSurface);


	// ***** Set Label Position *****
s_label.x1 = TIME_X;
s_label.y1 =  TIME_Y;
	// ***** put lable into text surface *****
p_TextSurface = TTF_RenderText_Blended(p_Font_Label, v_lable, v_color);
	// ***** Send text surface to the screen  *****
Setup_Coordinates(p_TextSurface, &v_SrcCoor, &v_DstCoor, &s_label);
SDL_BlitSurface(p_TextSurface, NULL, p_DisplayS, &v_DstCoor);
SDL_FreeSurface(p_TextSurface);
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****  	Close and Free up memory										********
TTF_CloseFont(p_Font_Data);
TTF_CloseFont(p_Font_Label);
//SDL_FreeSurface(p_DisplayS);

// ----------  END  ----------------------------------------------------------------

return;
}
// ----------  END DRAW_Depth ---------------------------------------------------



