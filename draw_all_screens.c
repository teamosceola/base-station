// *********************************************************************************
// *****	Christopher Crews 	05 Jan 2012									********
// *****	Program Name:	draw_all_screens.c								********
// *****	holds functions that retrieves data and draws each screen		********
// *********************************************************************************
// *****             Formated for Monospaced Type							********


// *********************************************************************************
// *****	Include Statments	 											********
#include "draw_all_screens.h"
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Draw_Nav_Screen													********
// *****	Calls all draw functions to draw the navigation screen			********
// *****	(r) int; 0 = success 1 = error									********
// *****	(1) SDL_Surface													********
// *********************************************************************************
int Draw_Nav_Screen(SDL_Surface* p_DisplayS, int v_screen, int v_distance, int v_bearing)
{
// *********************************************************************************
// *****	Veriable Declaratins 											********
int v_color = BLACK;
gps_struct strc_gps_data;
compass_struct strc_compass_data;
main_struct strc_main_data;

// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Retrieve Data		 											********
Get_GPS_Data(&strc_gps_data);
Get_Compass_Data(&strc_compass_data);
Get_Main_Data(&strc_main_data);
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Call all Drawing Functions										********
switch(v_screen)
{
	case 0:
	{
		Draw_Compass(p_DisplayS, strc_compass_data.course_heading, v_bearing,v_color);
		//Draw_Distance(p_DisplayS, v_distance, strc_gps_data.fix_type, strc_main_data.roc_fix_type);
		break;
	}
	
	case 1:
	{
		if(strc_gps_data.fix_type == 2 || strc_gps_data.fix_type == 3 || strc_gps_data.fix_type == 1)
		{
			strc_gps_data.fix_type = strc_gps_data.fix_type;
		}
		else
		{
			strc_gps_data.fix_type = 0;
		}

		Draw_Lat(p_DisplayS, strc_gps_data.latitude, strc_gps_data.fix_type);
		Draw_Long(p_DisplayS, strc_gps_data.longitude, strc_gps_data.fix_type);
		Draw_Time(p_DisplayS,strc_gps_data.hours,strc_gps_data.minutes,strc_gps_data.seconds);
		Draw_Fix(p_DisplayS,strc_gps_data.fix_type,strc_gps_data.num_sats);
		break;
	}
	
	case 2:
	{
		if(strc_main_data.roc_fix_type == 2 || strc_main_data.roc_fix_type == 3 || strc_main_data.roc_fix_type == 1)
		{
			strc_main_data.roc_fix_type = strc_main_data.roc_fix_type;
		}
		else
		{
			strc_main_data.roc_fix_type = 0;
		}

		Draw_Lat(p_DisplayS, strc_main_data.rocket_lat, strc_main_data.roc_fix_type);
		Draw_Long(p_DisplayS, strc_main_data.rocket_lon, strc_main_data.roc_fix_type);
		Draw_Time(p_DisplayS,strc_gps_data.hours,strc_gps_data.minutes,strc_gps_data.seconds);
		Draw_Fix(p_DisplayS,strc_main_data.roc_fix_type,strc_main_data.roc_num_sats);
		break;
	}
	default:
	{
		//exit(0);
		break;
	}
}


SDL_Flip(p_DisplayS);
// ----------  END  ----------------------------------------------------------------
return 0;
}

