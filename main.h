// *********************************************************************************
// *****	main.h															********
// *****	Christopher Crews	-	March 2012								********
// *********************************************************************************
#ifndef _MAINTHREAD
#define _MAINTHREAD


// *********************************************************************************
// *****	Include Statments	 											********

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_framerate.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>


#include "program_constants.h"
#include "compass.h"
#include "i2c_functions.h"
#include "uart_functions.h"
#include "gps.h"
#include "accel.h"
#include "xbee.h"

//#include "draw_functions.h"
//#include "draw_compass.h"
#include "draw_all_screens.h"
//#include "draw_fix.h"
//#include "draw_Lat.h"
//#include "draw_Long.h"
//#include "draw_time.h"
//#include "draw_distance.h"
// ----------  END  ----------------------------------------------------------------

typedef struct main_struct
{
	double rocket_lat, rocket_lon;
	int roc_fix_type, roc_num_sats, roc_data_valid;
	double base_lat, base_lon;
	int base_data_valid;
	int v_distance, v_bearing;
} main_struct;

int printall, v_orient, calibrating_compass;
#define FPS 27

#define GPS				(int)('G'*'P'*'S')
#define DAT				(int)('D'*'A'*'T')

// *********************************************************************************
// *****	Function Prototypes 											********

// *****  < main_thread >  *****
// return: void
// (1) argument type: void pointer
void *main_thread(void *ptr);
void Blank_Screen(SDL_Surface *p_DisplayS, int v_color);
void Parse_Data_String(unsigned char [], int, main_struct *);
void Get_Main_Data(main_struct *);
int Calculate_Bearing(double,double,double,double);
int Calculate_Distance(double,double,double,double);

// --------  END  ------------------------------------------------------------------

#endif  /* _MAINTHREAD  */
