// *********************************************************************************
// *****	Christopher Crews	-	February 2012							********
// *****	Program Name:		main.c										********
// *****	main - initalizes all the hardware and veriables.  it sets up	********
// *****	and starts all the threads.	this c file also holds the Main_Thread.*****
// *********************************************************************************
// *****             Formated for Monospaced Type							********
#ifdef _GUM

// *********************************************************************************
// *****	Include Statments	 											********
#include "main.h"
// ----------  END  ----------------------------------------------------------------

int v_i2c_device, v_uart_device;
main_struct strc_main;
// *********************************************************************************
// *****	Main															********
// *****																	********
// *********************************************************************************
int main (int argc, char **argv)
{

// *********************************************************************************
// *****	Veriable Declaratins 											********
	int v_returnvalue;
	pthread_t main_thread_ID;
	pthread_t compass_thread_ID;
	pthread_t gps_thread_ID;
	pthread_t accel_thread_ID;
	pthread_t xbee_thread_ID;
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	OPEN I2C PORT													********
	v_i2c_device = i2c_open();
	i2c_print_messages(OFF);
	if (v_i2c_device == 0)
	{
		printf("error on opening i2c port...exiting program.\n\n");
		return 0;
	}
	
	v_uart_device = uart_open();
	uart_print_messages(OFF);
	if(v_uart_device == 0)
	{
		printf("error on opening uart port...exiting program.\n\n");
	}
// ----------  END  ----------------------------------------------------------------
 
// *********************************************************************************
// *****	Save I2C device ID in all files that use I2C					********
	save_compass_i2c_device_id(v_i2c_device);
	save_gps_i2c_device_id(v_i2c_device);
	save_accel_i2c_device_id(v_i2c_device);
	save_xbee_uart_device_id(v_uart_device);
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Set Compass Orientation											********
	//v_orient = Set_Compass_Orintation(LEVEL);
	system("cpufreq-set -f 600MHz");
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	create threads													********
	v_returnvalue = pthread_create(&main_thread_ID, NULL, main_thread, NULL);
	v_returnvalue = pthread_create(&gps_thread_ID, NULL, GPS_Thread, NULL);
	v_returnvalue = pthread_create(&compass_thread_ID, NULL, Compass_Thread, NULL);
	//v_returnvalue = pthread_create(&accel_thread_ID, NULL, Accel_Thread, NULL);
	v_returnvalue = pthread_create(&xbee_thread_ID, NULL, Xbee_Thread, NULL);
	
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Closing Code													********
	pthread_join(main_thread_ID, NULL);
// ----------  END  ----------------------------------------------------------------

	return 0;
}
// ----------  END MAIN ------------------------------------------------------------


// *********************************************************************************
// *****	Function Name:  main_thread										********
// *****	Reads all data output from all the other threads				********
// *********************************************************************************
void *main_thread(void *ptr)
{
// *********************************************************************************
// *****	Veriable Declaratins 											********	
	SDL_Surface* p_DisplayS;
	SDL_Event Event;
	FPSmanager manager;
	compass_struct strc_compass_data;
	accel_struct strc_accel_data;
	gps_struct strc_gps_data;
	xbee_struct strc_xbee_data;
	main_struct strc_main_data;
	
	int v_color;
	v_color = BLACK;
	calibrating_compass = 0;
	printall = 0;
	int printall_gps = 0;
	int printall_comp = 0;
	int i = 0;
	int v_screen = 0;
	unsigned char v_string[2000];
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Initialize SDL		 											********
	SDL_Init(SDL_INIT_EVERYTHING);
// ----------  END Initialize SDL --------------------------------------------------

// *********************************************************************************
// *****	Set Video Mode		 											********
	p_DisplayS = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,8,SDL_HWSURFACE);
	if(p_DisplayS==NULL){
		printf("FAILURE!....SDL_SetVideoMode failed!\n");
		}
// ----------  END Set Video Mode --------------------------------------------------

// *********************************************************************************
// *****	Turn off cursor		 											********
	SDL_ShowCursor(SDL_DISABLE);
// ----------  END Turn off cursor -------------------------------------------------

// *********************************************************************************
// *****	Initialize framerate manager									********
	SDL_initFramerate(&manager);
	SDL_setFramerate(&manager,FPS);
// ----------  END Initialize framerate manager ------------------------------------

// *********************************************************************************
// *****	Initialize Draw Compass											********
	Init_Draw_Compass();
	TTF_Init();
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	MAIN LOOP														********

	for(;;)
	{
		SDL_framerateDelay(&manager);
		
		if(new_compass_data == 1)
		{
			Get_Compass_Data(&strc_compass_data);
			if (printall_comp)
			{
			printf("Heading = %3d degrees.\n",strc_compass_data.course_heading);
			printf("Damped Heading = %3.1f degrees.\n",strc_compass_data.damp_heading);
			printf("	Pitch = %2.2f degrees.\n",strc_compass_data.damp_pitch);
			printf("	Roll =  %3.2f degrees.\n",strc_compass_data.damp_roll);
			}
		}
		
		if(new_accel_data == 1)
		{
			Get_Accel_Data(&strc_accel_data);
			if(printall)
			{
			printf("Ax = %d\n",strc_accel_data.v_Ax);
			printf("Ay = %d\n",strc_accel_data.v_Ay);
			printf("Az = %d\n",strc_accel_data.v_Az);
			}
		}
			
		if(new_gps_data == 1)
		{
			Get_GPS_Data(&strc_gps_data);
			strc_main_data.base_lat = (double)(strc_gps_data.latitude);
			strc_main_data.base_lon = (double)(strc_gps_data.longitude);
			strc_main_data.base_data_valid = strc_gps_data.data_valid;
			if(printall_gps)
			{
			printf("\nLat = %.06f,  Long = %.06f\n",strc_gps_data.latitude,strc_gps_data.longitude);
			printf("Altitude = %.02f m, Speed = %.02f km/h\n",strc_gps_data.altitude,strc_gps_data.ground_speed);
			printf("Course over Ground = %.02f deg.\n",strc_gps_data.ground_course);
			printf("Number of Satellites = %d\n", strc_gps_data.num_sats);
			printf("Date: %d/%d/%d \n",strc_gps_data.day,strc_gps_data.month,strc_gps_data.year);
			printf("UTC Time: %d:%d:%d \n",strc_gps_data.hours,strc_gps_data.minutes,strc_gps_data.seconds);
			printf("Fix Type: %d\n\n", strc_gps_data.fix_type);
			printf("%d\n",i);
			i++;
			}
		}
		
		if(new_xbee_data == 1)
		{
			Get_Xbee_Data(&strc_xbee_data);
			Parse_Data_String(strc_xbee_data.v_rx_data,strc_xbee_data.v_length,&strc_main_data);
			strc_main_data.v_bearing = Calculate_Bearing(strc_main_data.base_lat, strc_main_data.base_lon, strc_main_data.rocket_lat, strc_main_data.rocket_lon);
			printf("Bearing = %d\n",strc_main_data.v_bearing);
			//strc_main_data.v_distance = Calculate_Distance(strc_main_data.base_lat, strc_main_data.base_lon, strc_main_data.rocket_lat, strc_main_data.rocket_lon);
			printf("%s\n",strc_xbee_data.v_rx_data);
		}
		
		if(SDL_PollEvent(&Event)!=0)
		{
			if(Event.type==SDL_QUIT)
			{
				break;
			}
			else if(Event.type == SDL_MOUSEBUTTONDOWN)
			{
				v_screen++;
				if(v_screen > 2) v_screen = 0;
			}
			
		}
		
		strc_main = strc_main_data;
		Blank_Screen(p_DisplayS,v_color);
		Draw_Nav_Screen(p_DisplayS, v_screen, strc_main_data.v_distance, strc_main_data.v_bearing);
	
	}
// ----------  END  ----------------------------------------------------------------

	SDL_FreeSurface(p_DisplayS);
	i2c_close(v_device);
	TTF_Quit();
	
	// Quit SDL
	SDL_Quit();
	
	return 0;

}
// ----------  END MAIN_THREAD  ----------------------------------------------------

void Blank_Screen(SDL_Surface *p_DisplayS, int v_color)
{
	if(v_color == WHITE)
	{
		boxRGBA(p_DisplayS,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,255,255,255,255); // White Background
	}
	else if(v_color == BLACK)
	{
		boxRGBA(p_DisplayS,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0,0,0,255); // Black Background
	}	
	else
	{
		boxRGBA(p_DisplayS,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0,0,0,255); // Default: Black Background
	}
}

void Parse_Data_String(unsigned char v_string[], int v_length, main_struct *temp)
{
	int j, k, x;
	int string_ID = v_string[3]*v_string[4]*v_string[5];
	
	FILE *pFile;
	
	
	switch ( string_ID ) 
	{
		case GPS:
		{
			// GPS has 7 data fields. Allocate memory for each field
			unsigned char **field;
			field = (unsigned char **)malloc(7 * sizeof(unsigned char *));
			while(field == NULL)
			{
				field = (unsigned char **)malloc(7 * sizeof(unsigned char *));
			}
			for(j=0;j<7;j++)
			{
				field[j] = malloc(20 * sizeof(unsigned char));
				while(field[j] == NULL)
				{
					field[j] = (unsigned char *)malloc(7 * sizeof(unsigned char));
				}
			}
			
			// Zero out the fields
			for(j=0;j<7;j++)
			{
				for(k=0;k<20;k++)
				{
					field[j][k] = 0;
				}
			}
			
			// Separate the string into individual data fields
			k = x = 0;
			for(j=0;j<v_length;j++)
			{
				if(v_string[j] != ',')
				{
					field[k][x] = v_string[j];
					x++;
				}
				else
				{
					field[k][x] = '\0';
					k++;
					x = 0;
				}
			}
			
			// Extract Latitude
			temp->rocket_lat = atof(field[1]);
			// Extract Longitude
			temp->rocket_lon = atof(field[2]);
			// Extract Fix Type
			temp->roc_fix_type = atoi(field[4]);
			//Extract # of Sat's
			temp->roc_num_sats = atoi(field[5]);
			//Extract data valid flag
			temp->roc_data_valid = atoi(field[6]);
			
			for(j=0;j<7;j++)
			{
				free(field[j]);
			}
			free(field);
			break;
		}
		case DAT:
		{
			pFile = fopen("//home/root/Programs/results.csv","a");
			fprintf(pFile,"%s",v_string);
			fclose(pFile);
			break;
		}
		default:
		{
			break;
		}
	} // End of Switch Statement
}

void Get_Main_Data(main_struct *temp)
{
	*temp = strc_main;		// ***** copy data into tmp struct
}

int Calculate_Bearing(double latA,double lonA,double latB,double lonB)
{
	double delta_lon, delta_phi, theta, num, den;
	int bearing;
	
	delta_lon = lonB - lonA;
	num = tan((latB/2) + (M_PI/4));
	den = tan((latA/2) + (M_PI/4));
	delta_phi = log(num/den);
	theta = atan(delta_lon/delta_phi);
	if(theta >= 0)
	{
		bearing = (int)(theta*180/M_PI);
	}
	else
	{
		bearing = (int)((theta*180/M_PI)+360);
	}
	return bearing;
}

int Calculate_Distance(double latA,double lonA,double latB,double lonB)
{
	double delta_lon, phi, x, y;
	int distance;
	double R = 20925524.9;
	delta_lon = lonB - lonA;
	x = cos(latA)*cos(latB)*cos(delta_lon);
	y = sin(latA)*sin(latB);
	phi = acos(x + y);
	distance = (int)(R*phi);
	return distance;
}

#endif /* _GUM  */




