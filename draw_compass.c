// *********************************************************************************
// *****	Christopher Crews 	21 July 2011								********
// *****	Program Name:	draw_compass_1.c	    						********
// *****	Draws a circular compass with heading arrow and bearing line	********
// *****	Note: Bitmaps must be 8-bit/256 color format bitmaps.			********
// *********************************************************************************
// *****             Formated for Monospaced Type							********

// *********************************************************************************
// *****	Include Statments	 											********
#include "draw_compass.h"
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Variable Declarations 											********
static int v_delta_x[361], v_delta_y[361];
// ----------  End Variable Declarations -------------------------------------------


// *********************************************************************************
// *****	Init_Draw_Compass Function										********
// *****	Draws a circular compass with heading arrow and bearing line	********
// *****	(r) none:  void													********
// *****	(1) void														********
// *********************************************************************************
void Init_Draw_Compass(void)
{
	int i = 0;
	for(i=0;i<=360;i++)
	{
		// Get rotated surfaces sizes
		rotozoomSurfaceSize(COMPASS_WIDTH,COMPASS_HEIGHT,i,ZOOM,&v_delta_x[i],&v_delta_y[i]);
		
		// Calculate the difference in x & y positions for the source rectangle. 
		// Is the shift in x & y of the source rectangle to keep the center of the 
		// at a fixed point inside the rectangle so that when blitted, the center of 
		// the compass maintains a fixed position on the screen.
		v_delta_x[i] = (((v_delta_x[i] - COMPASS_WIDTH)/2) + ((COMPASS_WIDTH/2) - (RADIUS+2)));
		v_delta_y[i] = (((v_delta_y[i] - COMPASS_HEIGHT)/2) + ((COMPASS_WIDTH/2) - (RADIUS+2)));
	}
}
// ----------  END Draw_Compass_Init -----------------------------------------------



// *********************************************************************************
// *****	Draw_Compass Function											********
// *****	Draws a circular compass with heading arrow and bearing line	********
// *****	(r) none:  void													********
// *****	(1) SDL_Surface *: Main display surface							********
// *****	(2) int: Current heading in degrees								********
// *****	(3) int: Current bearing in degrees								********
// *****	(4) int: Background color										********
// *********************************************************************************
void Draw_Compass(SDL_Surface *p_DisplayS, int v_heading, int v_bearing, int v_color)
{
// *********************************************************************************
// *****	Variable Declaratins 											********
	SDL_Surface* p_BitmapSurface;
	SDL_Surface* p_CompassSurface;
	SDL_Rect v_SrcRect, v_DstRect;
	short v_x[4], v_y[4];	// x & y positions for points of bearing line polygon
	double v_angle, v_theta_1, v_theta_2; // angles used in calculating bearing line
	int v_error_flag;
// ----------  END Variable Declarations -------------------------------------------


// *********************************************************************************
// *****	Initialize Varialbes											********
	v_error_flag = 0;
// ----------  END Initialize Variables --------------------------------------------


// *********************************************************************************
// *****	Erase Compass													********
	Erase_Compass(p_DisplayS, v_color);
// ----------  END Erase Compass ---------------------------------------------------


// *********************************************************************************
// *****	Heading Arrow Bitmap 											********
#ifdef _GUM
	// Load Heading Arrow Bitmap
	p_BitmapSurface = SDL_LoadBMP("//home/root/Programs/Graphics/heading_arrow.bmp");
	if(p_BitmapSurface==NULL){
		printf("FAILURE!....Heading Arrow Bitmap = NULL\n");
		}
#endif
#ifdef _PC
	// Load Heading Arrow Bitmap
	p_BitmapSurface = SDL_LoadBMP("Graphics/heading_arrow.bmp");
	if(p_BitmapSurface==NULL){
		printf("FAILURE!....Heading Arrow Bitmap = NULL\n");
		}
#endif
	// Set ColorKey for Heading Arrow
	v_error_flag = SDL_SetColorKey(p_BitmapSurface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(p_BitmapSurface->format, COLORKEY));
	if(v_error_flag != 0){
		printf("FAILURE!....SDL_SetColorKey() for heading arrow failed\n");
		v_error_flag = 0;
		}
		
	// Setup Heading Arrow Destination Rectangle for Blitting
	// No source rectangle is needed b/c entire surface is being blitted
	v_DstRect.x = HEAD_ARROW_X;
	v_DstRect.y = HEAD_ARROW_Y;
	v_DstRect.w = p_BitmapSurface->w;
	v_DstRect.h = p_BitmapSurface->h;
	
	// Blit Heading Arrow to main display surface
	v_error_flag = SDL_BlitSurface(p_BitmapSurface,NULL,p_DisplayS,&v_DstRect);
	if(v_error_flag != 0){
		printf("FAILURE!....SDL_BlitSurface() failed (heading arrow)\n");
		v_error_flag = 0;
		}
	
	// Free Compass Surface
	SDL_FreeSurface(p_BitmapSurface);

// ----------  END Heading Arrow Bitmap --------------------------------------------


// *********************************************************************************
// *****	Compass Bitmap													********

	// Load Compass Bitmap
	// Note: SetColorKey is not needed for this bitmap surface b/c it is saved to 
	// a new surface during rotation called "p_CompassSurface", and this is the surface
	// that will need the ColorKey to be set.
#ifdef _GUM
	p_BitmapSurface = SDL_LoadBMP("//home/root/Programs/Graphics/circular_compass.bmp");
	if(p_BitmapSurface==NULL){
		printf("FAILURE!....Compass Bitmap = NULL\n");
		}
#endif
#ifdef _PC
	p_BitmapSurface = SDL_LoadBMP("Graphics/circular_compass.bmp");
	if(p_BitmapSurface==NULL){
		printf("FAILURE!....Compass Bitmap = NULL\n");
		}
#endif
// *********************************************************************************
// *****	Calculate & Draw bearing line      								********
	
	// Calculate points for bearing line. Bearing line is created using a 4 point
	// filled polygon.
	
	// angle of bearing line
	v_angle = (double)((360 - v_bearing+90)*M_PI/180);	// Note: 90 deg. is added to the
	// bearing to accomodate compass coordinates rather than polar coordinates.
	
	// difference in angle of bearing line and points # 1 & 2
	v_theta_1 = atan(BEAR_LINE_W/RADIUS);
	
	//difference in anble of bearing line and points # 3 & 4
	v_theta_2 = atan(BEAR_LINE_W/BEAR_LINE_W);
	
	// point #1
	v_x[0] = (short)(round(RADIUS*cos(v_angle+v_theta_1) + BITMAP_CENTER));
	v_y[0] = (short)(round(BITMAP_CENTER-RADIUS*sin(v_angle+v_theta_1)));
	// point #2
	v_x[1] = (short)(round(RADIUS*cos(v_angle-v_theta_1) + BITMAP_CENTER));
	v_y[1] = (short)(round(BITMAP_CENTER-RADIUS*sin(v_angle-v_theta_1)));
	// point #3
	v_x[2] = (short)(round(BEAR_LINE_W*cos(v_angle-v_theta_2) + BITMAP_CENTER));
	v_y[2] = (short)(round(BITMAP_CENTER-BEAR_LINE_W*sin(v_angle-v_theta_2)));
	// point #4
	v_x[3] = (short)(round(BEAR_LINE_W*cos(v_angle+v_theta_2) + BITMAP_CENTER));
	v_y[3] = (short)(round(BITMAP_CENTER-BEAR_LINE_W*sin(v_angle+v_theta_2)));
	
	// Draw Bearing line on compass
	v_error_flag = filledPolygonRGBA(p_BitmapSurface,v_x,v_y,(int)(N),255,0,0,255);
	if(v_error_flag != 0){
		printf("FAILURE!.....filledPolygonRGBA() for bearing line failed!\n");
		v_error_flag = 0;
		}
		
// ----------	END ----------------------------------------------------------------
	
		
	// Rotate Compass to desired heading (bearing line alread on compass)
	// Note: the bitmap surface is being rotated and saved as a new surface.
	p_CompassSurface = rotozoomSurface(p_BitmapSurface, v_heading, ZOOM, SMOOTHING_ON);
	
	// Free Bitmap Surface
	SDL_FreeSurface(p_BitmapSurface);

	// Setup Source and Destination Rectangles for Blitting
	// Source Rectangle
	v_SrcRect.x = v_delta_x[v_heading];
	v_SrcRect.y = v_delta_y[v_heading];
	v_SrcRect.w = COMPASS_VISIBLE_WIDTH;
	v_SrcRect.h = COMPASS_VISIBLE_HEIGHT;
	// Destination Rectangle
	v_DstRect.x = COMPASS_X;
	v_DstRect.y = COMPASS_Y;
	v_DstRect.w = COMPASS_VISIBLE_WIDTH;
	v_DstRect.h = COMPASS_VISIBLE_HEIGHT;
	
	// Set ColorKey for compass
	v_error_flag = SDL_SetColorKey(p_CompassSurface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(p_CompassSurface->format, COLORKEY));
	if(v_error_flag != 0){
		printf("FAILURE!....SDL_SetColorKey() for compass Failed\n");
		v_error_flag = 0;
		}
	
	// Blit rotated compass to main display surface
	v_error_flag = SDL_BlitSurface(p_CompassSurface,&v_SrcRect,p_DisplayS,&v_DstRect);
	if(v_error_flag != 0){
		printf("FAILURE!....SDL_BlitSurface() failed (compass)\n");
		v_error_flag = 0;
		}
	
	// Free Compass Surface
	SDL_FreeSurface(p_CompassSurface);

// ----------  END Compass Bitmap --------------------------------------------------


// *********************************************************************************
// *****	Update display surface											********
	
	// Flip Display
	//SDL_Flip(p_DisplayS);
	
// ----------  END Update display surface ------------------------------------------

}
// ----------  END Draw_Compass Function -------------------------------------------



// *********************************************************************************
// *****	Erase_Compass Function											********
// *****	Blanks out compass with given color								********
// *****	(r) none:  void													********
// *****	(1) SDL_Surface *: Main display surface							********
// *****	(2) int: color													********
// *********************************************************************************
void Erase_Compass(SDL_Surface *p_DisplayS, int v_color)
{
	if(v_color == WHITE)
	{
		boxRGBA(p_DisplayS,COMPASS_X,HEAD_ARROW_Y,(COMPASS_X + COMPASS_VISIBLE_WIDTH),(COMPASS_Y + COMPASS_VISIBLE_HEIGHT),255,255,255,255); // White Background
	}
	else if(v_color == BLACK)
	{
		boxRGBA(p_DisplayS,COMPASS_X,HEAD_ARROW_Y,(COMPASS_X + COMPASS_VISIBLE_WIDTH),(COMPASS_Y + COMPASS_VISIBLE_HEIGHT),0,0,0,255); // Black Background
	}	
	else
	{
		boxRGBA(p_DisplayS,COMPASS_X,HEAD_ARROW_Y,(COMPASS_X + COMPASS_VISIBLE_WIDTH),(COMPASS_Y + COMPASS_VISIBLE_HEIGHT),0,0,0,255); // Default: Black Background
	}
}
// ----------  END Erase_Compass ---------------------------------------------------




