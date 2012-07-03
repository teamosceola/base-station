// *********************************************************************************
// *****	< compass.h>													********
// *****	Richard Manley	-	10 January 2010								********
// *********************************************************************************
#ifdef _GUM
#ifndef _COMPASS
#define _COMPASS


// *********************************************************************************
// *****	Include Statments	 											********
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "i2c_functions.h"
#include "draw_compass.h"
#include "main.h"
// ----------  END  ----------------------------------------------------------------



// *********************************************************************************
// *****	Constants			 											********
#define COMPASS_ADDRESS	0X42
#define LEVEL 	1
#define UPRIGHT_EDGE	2
#define UPRIGHT_FRONT	3
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Veriable Declaratins 											********
typedef struct compass_struct
{
	float 	heading;
	float	damp_heading;
	float 	pitch, damp_pitch, hist_pitch[10];
	float 	roll, damp_roll, hist_roll[10];
	float	Ax, damp_Ax, hist_Ax[6];
	float	Ay, damp_Ay, hist_Ay[6];
	float	Az, damp_Az, hist_Az[6];
	float	Mx, damp_Mx, hist_Mx[6];
	float	My, damp_My, hist_My[6];
	float	Mz, damp_Mz, hist_Mz[6];
	float 	historic_readings[10];
	int 	course_heading;
	int 	status;
	char	current_cal_data[6];
	char	previous_cal_data[6];
	float	CH, CP, CR;
	char	orient[2];
}compass_struct;
	// ***** Global veriable 'new data' flag *****
int new_compass_data;
int v_device;
// --------  END  ------------------------------------------------------------------

// *********************************************************************************
// *****	Function Prototypes 											********

// *****  Compass_Thread  *****
// return: void
// (1) argument type: void pointer
void *Compass_Thread(void *ptr);
// --------  END  ------------------------------------------------------------------


// *****  Get_Compass_Data  *****
// return: void
// (1) pointer to a bottle_struct
void Get_Compass_Data(compass_struct *);
// --------  END  ------------------------------------------------------------------


// *****  Get_Heading_Data  *****
// return: void
// (1) pointer to a bottle_struct
void Get_Heading_Data(compass_struct *);
// --------  END  ------------------------------------------------------------------


// *****  Save_I2C_device_ID  *****
// return: void
// (1) int:  device ID
void save_compass_i2c_device_id(int);
// ----------  END  ----------------------------------------------------------------


// *****  Set Compass Orintation  *****
// return: void
// (1) int orintation
// takes in an int represinting an orintation and 
// writes to the compass to set the correct orintation
int Set_Compass_Orintation(int);
// --------  END  ------------------------------------------------------------------


// *****  Array to 16 bit int  *****
// return: voltage
// (1) int byte one
// (2) int byte two
// (3) int byte three
// takes three bytes and converts to 32 bin integer
int array_to_16bit_int(unsigned int, unsigned int);
// --------  END  ------------------------------------------------------------------


// *****  Calculate_Heading  *****
// return: void
// (1) float:  raw_heading
// takes the heading reading and 
// calculates a calibrated/buffered heading.
void Calculate_Heading(int data[], compass_struct *);
// --------  END  ------------------------------------------------------------------


// *****  Calculate_Acceleration  *****
// return: void
// (1) float:  raw_compass data
// (2) compass_struct:  structure holding compass veriables
// takes the raw 6 byte data array and parces it into 3 axis Accel data
void Calculate_Acceleration(int data[], compass_struct *);
// --------  END  ------------------------------------------------------------------


// *****  Calculate_Magnetic  *****
// return: void
// (1) float:  raw_compass data
// (2) compass_struct:  structure holding compass veriables
// takes the raw 6 byte data array and parces it into 3 axis Accel data
void Calculate_Magnetic(int data[], compass_struct *);
// --------  END  ------------------------------------------------------------------


// *****  Set_Course_Heading *****
// return: void
// (1) float:  raw_heading
// (2) compass_struct *:  structure to save data to
// takes the current heading and saves it as the course 
void Set_Course_Heading();
// --------  END  ------------------------------------------------------------------


// *****  Calibrate_Compass *****
// (r) void
// (1) void
void Calibrate_Compass(void);
void Init_Calibrate_Compass(void);
void End_Calibrate_Compass(void);
// --------  END  ------------------------------------------------------------------

// ***** Print_Calibration_Data *****
void Print_Calibration_Data(char str[]);
void Read_Calibration_Data(void);
// --------  END  ------------------------------------------------------------------

#endif  /* _COMPASS  */
#endif  /* _GUM  */
