// *********************************************************************************
// *****	Christopher Crews	-	10 January 2010							********
// *****	Program Name:		compass.c									********
// *****	the compass.c file contains all the functions					********
// *****	to read the compass i2c data									********
// *********************************************************************************
// *****             Formated for Monospaced Type							********
#ifdef _GUM
// *********************************************************************************
// *****	Include Statments	 											********
#include "compass.h"
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Veriable Declaratins 											********
compass_struct strc_compass;
// ----------  END  ----------------------------------------------------------------
 

// *********************************************************************************
// *****	Compass_Thread													********
// *****	continous thread that reads and calulates compass heading		********
// *****	(r) void														********
// ***** 	(1) void *: pointer to any data						            ********
// *********************************************************************************
void *Compass_Thread(void *ptr)
{
// *********************************************************************************
// *****	Veriable Declaratins 											********
int i;
int v_iteration_number;
compass_struct strc_temp_compass_data;
int v_return_test;
int v_data_count;
unsigned char v_Register_Address;
unsigned char v_Compass_Array[6];
int a_compass[6];

// ----------  END  ----------------------------------------------------------------

/*(printf("Entering Compass calibration Mode\n");
Init_Calibrate_Compass();
usleep(60000000);
printf("Exiting compass calibration mode\n");
End_Calibrate_Compass();*/

// *********************************************************************************
// *****	Main Thread Loop												********
for(;;)
{
	if(!calibrating_compass)
	{
// *********************************************************************************
// *****	Read compass													********
	v_Register_Address = 'A';
	v_data_count = 0;
	v_return_test = i2c_write(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Array, v_data_count);
	usleep(6000);
	v_data_count = 2;
	v_return_test = i2c_read(v_device, COMPASS_ADDRESS, SKIP_WRITE_REGISTER, v_Compass_Array, v_data_count);
	a_compass[0] = array_to_16bit_int(v_Compass_Array[0], v_Compass_Array[1]);
	
	Calculate_Heading(a_compass, &strc_temp_compass_data);
// ----------  END  ----------------------------------------------------------------
	
	strc_compass = strc_temp_compass_data;
	v_iteration_number++;
	new_compass_data = 1;
	}
	usleep(100000);
}
// ----------  END  ----------------------------------------------------------------
}
// ----------  END COMPASS_THREAD  -------------------------------------------------


// *********************************************************************************
// *****	Get_Compass_Data												********
// *****	this function is used to get the data out of the 				********
// *****	compass.c file and back to the calling routine					********
// *****	(r) void														********
// ***** 	(1) compass_struct *: pointer to a button_struct		        ********
// *********************************************************************************
void Get_Compass_Data(compass_struct *temp)
{
	*temp = strc_compass;		// ***** copy data into tmp struc
	new_compass_data = 0;		// ***** set new data flag
}
// ----------  END GET_COMPASS_DATA  -----------------------------------------------


// *********************************************************************************
// *****	Save_I2C_device_ID												********
// *****	this functions receives and saves the I2C device ID number		********
// *****	(r) void														********
// ***** 	(1) int: device ID									            ********
// *********************************************************************************
void save_compass_i2c_device_id(int I2C_ID)
{
	v_device = I2C_ID;
	return;
}
// ----------  END SAVE_I2C_DEVICE_ID ----------------------------------------------


// *********************************************************************************
// *****	Set_Compass_Orintation											********
// *****	writes to the compass to set the correct orintation				********
// *****	(r) void														********
// ***** 	(1) int: 1 = Level 2 = Upright Edge 3 = Upright Front           ********
// *********************************************************************************
int Set_Compass_Orintation(int v_orintation)
{
// *********************************************************************************
// *****	Veriable Declaratins 											********
	int v_data_count = 2;
	int v_orient;
	unsigned char v_data[2];
	unsigned char v_register;
	int v_return_test;
	v_register = 0xF1;
	v_data[0] = 0x04;
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Set orintation register value, write to compass, and reset		********
	switch(v_orintation)
	{
		case 1:	v_data[1] = 0x72;
				v_orient = LEVEL;
				break;
		case 2: v_data[1] = 0x73;
				v_orient = UPRIGHT_EDGE;
				break;
		case 3: v_data[1] = 0x74;
				v_orient = UPRIGHT_FRONT;
				break;
	}
	printf("initalizing compass with orientation:  %#02X\n", v_data[1]);			
	v_return_test = i2c_write(v_device, COMPASS_ADDRESS, v_register, v_data, v_data_count);
	if(!v_return_test) printf("Error in compass write.\n");
	else printf("DONE.\n");
// ----------  END  ----------------------------------------------------------------
	
	return v_orient;
}
// ----------  END Set Compass Orintation ------------------------------------------


// *********************************************************************************
// *****	Array to 16 bit int												********
// *****	takes two bytes and converts to 32 bin integer					********
// *****    (r) int: combination of two bytes in signed formati				********
// *****	(1) int byte two												********
// *****	(2) int byte one												********
// *********************************************************************************
int array_to_16bit_int(unsigned int v_two, unsigned int v_one)
{
// *********************************************************************************
// *****	Veriable Declaratins 											********
short v_int_heading;
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	convert raw numbers												********
	// ***** push 2 btyes into one 16 bit number ******
v_int_heading = (v_two * 256) + v_one;
// ----------  END  ----------------------------------------------------------------
return (int)v_int_heading;
}	
// ----------  END ARRAY_TO_16BIT_INT  ---------------------------------------------


// *********************************************************************************
// *****	Calculate_Heading												********
// *****	takes the last reading and 										********
// *****	calculates the heading and moving average						********
// *****	(r) void														********
// ***** 	(1) float:  voltage heading									    ********
// *********************************************************************************
void Calculate_Heading(int a_data[], compass_struct *temp)
{
// *********************************************************************************
// *****	Veriable Declaratins 											********
int i = 4;
float sum = 0;
// ----------  END  ----------------------------------------------------------------

temp->heading = (a_data[0] / 10.0);

// *********************************************************************************
// *****	condition raw compass data										********
	// ***** update historic readings array *****
for(i = 7; i >= 1; i--)
{
	temp->historic_readings[i] = temp->historic_readings[i - 1];
}
temp->historic_readings[0] = temp->heading;
	// ***** saving 10 point moving average as heading *****
for(i = 0; i<=7; i++)
{
	sum = sum + temp->historic_readings[i];
}
if(abs(temp->historic_readings[0]-temp->historic_readings[1]) > 20){
	temp->damp_heading = temp->heading;
	}
else if(abs(temp->historic_readings[1]-temp->historic_readings[2]) > 20){
	temp->damp_heading = temp->heading;
	}
else if(abs(temp->historic_readings[2]-temp->historic_readings[3]) > 20){
	temp->damp_heading = temp->heading;
	}
else if(abs(temp->historic_readings[3]-temp->historic_readings[4]) > 20){
	temp->damp_heading = temp->heading;
	}
else {
	temp->damp_heading = sum / 8.0;
	}
// ---------------------------------------------------------------------------------



temp->course_heading = 360 - (int)(round(temp->heading));

// ----------  END  ----------------------------------------------------------------
return;
}
// ---------  END CALCULATE_HEADING ------------------------------------------------


// *********************************************************************************
// *****	Calculate_Acceleration											********
// *****	takes the last reading and 										********
// *****	calculates the 3 axis acceleration								********
// *****	(r) void														********
// ***** 	(1) int:   data array with raw compass data					    ********
// *****	(2) compass_struct:  holds parced accel data					********
// *********************************************************************************
void Calculate_Acceleration(int a_data[], compass_struct *temp)
{
int i;
float sum;
// *********************************************************************************
// *****	condition raw compass data										********
temp->Ax = (float)(a_data[0]/1000.0);
// *********************************************************************************

sum = 0;
for (i = 5; i >= 1; i--)
{
	temp->hist_Ax[i] = temp->hist_Ax[i-1];
}
temp->hist_Ax[0] = temp->Ax;
for(i = 0; i <= 5; i++)
{
	sum = sum + temp->hist_Ax[i];
}
temp->damp_Ax = sum/6.0;
// ---------------------------------------------------------------------------------
temp->Ay = (float)(a_data[1]/1000.0);
// *********************************************************************************

sum = 0;
for (i = 5; i >= 1; i--)
{
	temp->hist_Ay[i] = temp->hist_Ay[i-1];
}
temp->hist_Ay[0] = temp->Ay;
for(i = 0; i <= 5; i++)
{
	sum = sum + temp->hist_Ay[i];
}
temp->damp_Ay = sum/6.0;
// ---------------------------------------------------------------------------------
temp->Az = (float)(a_data[2]/1000.0);
// *********************************************************************************

sum = 0;
for (i = 5; i >= 1; i--)
{
	temp->hist_Az[i] = temp->hist_Az[i-1];
}
temp->hist_Az[0] = temp->Az;
for(i = 0; i <= 5; i++)
{
	sum = sum + temp->hist_Az[i];
}
temp->damp_Az = sum/6.0;
// ---------------------------------------------------------------------------------

return;
}
// ---------  END CALCULATE_Acceleration--------------------------------------------


// *********************************************************************************
// *****	Calculate_Magnetic												********
// *****	takes the last reading and 										********
// *****	calculates the 3 axis of magnetic								********
// *****	(r) void														********
// ***** 	(1) int:   data array with raw compass data					    ********
// *****	(2) compass_struct:  holds parced accel data					********
// *********************************************************************************
void Calculate_Magnetic(int a_data[], compass_struct *temp)
{
int i;
float sum;
// *********************************************************************************
// *****	condition raw compass data										********
temp->Mx = (float)(a_data[0]/1000.0);
// *********************************************************************************

sum = 0;
for (i = 5; i >= 1; i--)
{
	temp->hist_Mx[i] = temp->hist_Mx[i-1];
}
temp->hist_Mx[0] = temp->Mx;
for(i = 0; i <= 5; i++)
{
	sum = sum + temp->hist_Mx[i];
}
temp->damp_Mx = sum/6.0;
// ---------------------------------------------------------------------------------
temp->My = (float)(a_data[1]/1000.0);
// *********************************************************************************

sum = 0;
for (i = 5; i >= 1; i--)
{
	temp->hist_My[i] = temp->hist_My[i-1];
}
temp->hist_My[0] = temp->My;
for(i = 0; i <= 5; i++)
{
	sum = sum + temp->hist_My[i];
}
temp->damp_My = sum/6.0;
// ---------------------------------------------------------------------------------
temp->Mz = (float)(a_data[2]/1000.0);
// *********************************************************************************

sum = 0;
for (i = 5; i >= 1; i--)
{
	temp->hist_Mz[i] = temp->hist_Mz[i-1];
}
temp->hist_Mz[0] = temp->Mz;
for(i = 0; i <= 5; i++)
{
	sum = sum + temp->hist_Mz[i];
}
temp->damp_Mz = sum/6.0;
// ---------------------------------------------------------------------------------

return;
}
// ---------  END Calculate_Magnetic -----------------------------------------------


// *********************************************************************************
// *****	Set_Course_Heading												********
// *****	takes the current heading as saves it as the course_heading		********
// *****	(r) void														********
// ***** 	(1) compass_struct *:  sturucture holdong the compass data	    ********
// *********************************************************************************
void Set_Course_Heading(void)
{
// *********************************************************************************
// *****	Veriable Declaratins 											********

// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Save the current heading or reset the course_heading			********
if (strc_compass.course_heading == 999)
{
	strc_compass.course_heading = (int)strc_compass.heading;
}
else
{
		// ***** 999 = no course_heading
	strc_compass.course_heading = 999;	
}
// ----------  END  ----------------------------------------------------------------
return;
}
// ---------  END SET_COURSE_HEADING -----------------------------------------------

// *********************************************************************************
// *****	Calibrate_Compass 												********
// *****	Enters Calibration mode for 30 secs. To Calibrate, the compass  ********
// *****	must be rotated 360 degrees about the Y-axis and then rotated   ********
// *****	360 degrees about the Z-axis. 									********
// *****	(r) void 														********
// *****	(1) void 														********
// *********************************************************************************
void Calibrate_Compass(void)
{
	unsigned char v_data[1];
	int v_return_test = 0;
	unsigned char v_register = 0x71;
	v_data[0] = 0x00;
	int v_data_count = 0;
	// Send command to enter calibration mode
	v_return_test = i2c_write(v_device, COMPASS_ADDRESS, v_register, v_data, v_data_count);
	if(!v_return_test) printf("Error in compass write (Enter Calibration).\n");
	// sleep for 30 seconds while user is rotating compass
	sleep(30);
	v_register = 0x7E;
	// Send command to exit calibration mode
	v_return_test = i2c_write(v_device, COMPASS_ADDRESS, v_register, v_data, v_data_count);
	if(!v_return_test) printf("Error in compass write (Exit Calibration).\n");
}
// ----------  END Calibrate_Compass -----------------------------------------------

// *********************************************************************************
// *****	Calibrate_Compass 												********
// *****	Enters Calibration mode for 30 secs. To Calibrate, the compass  ********
// *****	must be rotated 360 degrees about the Y-axis and then rotated   ********
// *****	360 degrees about the Z-axis. 									********
// *****	(r) void 														********
// *****	(1) void 														********
// *********************************************************************************
void Init_Calibrate_Compass(void)
{
	unsigned char v_data[1];
	int v_return_test = 0;
	unsigned char v_register = 'C';
	v_data[0] = 0x00;
	int v_data_count = 0;
	// Send command to enter calibration mode
	v_return_test = i2c_write(v_device, COMPASS_ADDRESS, v_register, v_data, v_data_count);
	if(!v_return_test) printf("Error in compass write (Enter Calibration).\n");
}
// ----------  END Calibrate_Compass -----------------------------------------------

// *********************************************************************************
// *****	Calibrate_Compass 												********
// *****	Enters Calibration mode for 30 secs. To Calibrate, the compass  ********
// *****	must be rotated 360 degrees about the Y-axis and then rotated   ********
// *****	360 degrees about the Z-axis. 									********
// *****	(r) void 														********
// *****	(1) void 														********
// *********************************************************************************
void End_Calibrate_Compass(void)
{
	unsigned char v_data[1];
	int v_return_test = 0;
	unsigned char v_register = 'E';
	v_data[0] = 0x00;
	int v_data_count = 0;
	// Send command to exit calibration mode
	v_return_test = i2c_write(v_device, COMPASS_ADDRESS, v_register, v_data, v_data_count);
	usleep(500000);
	if(!v_return_test) printf("Error in compass write (Exit Calibration).\n");
}
// ----------  END Calibrate_Compass -----------------------------------------------

// *********************************************************************************
// *****	Print_Calibration_Data											********
// *****	(r) void 														********
// *****	(1) void 														********
// *********************************************************************************
void Print_Calibration_Data(char str[])
{
	char str1[512];
	char str2[512];
	Read_Calibration_Data();
	sprintf(str1,"\n\nPrevious: %#2X  1: %#2X  2: %#2X  3: %#2X  4: %#2X   5: %#2X \n\n", strc_compass.previous_cal_data[0], strc_compass.previous_cal_data[1], strc_compass.previous_cal_data[2], strc_compass.previous_cal_data[3], strc_compass.previous_cal_data[4], strc_compass.previous_cal_data[5]);
	sprintf(str2,"Current: %#2X  1: %#2X  2: %#2X  3: %#2X  4: %#2X   5: %#2X \n", strc_compass.current_cal_data[0], strc_compass.current_cal_data[1], strc_compass.current_cal_data[2], strc_compass.current_cal_data[3], strc_compass.current_cal_data[4], strc_compass.current_cal_data[5]);
	sprintf(str,"%s%s",str1,str2);
}
// ----------  END Calibrate_Compass -----------------------------------------------

void Read_Calibration_Data(void)
{
	int v_data_count;
	int v_return_test;
	int i;
	unsigned char v_Register_Address;
	unsigned char v_Compass_Data[6];
	unsigned char v_Stored_Data[6];
	
	v_data_count = 1;
	v_Register_Address = 0xE1;

	v_Compass_Data[0] = 0x04;
	v_return_test = i2c_write(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Data, v_data_count);
	v_return_test = i2c_read(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Data, v_data_count);
	if (!v_return_test) printf("Error: Compass orientation setup\n\n");
	v_Stored_Data[0] = v_Compass_Data[0];

	v_Compass_Data[0] = 0x0E;
	v_return_test = i2c_write(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Data, v_data_count);
	v_return_test = i2c_read(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Data, v_data_count);
	if (!v_return_test) printf("Error: Compass orientation setup\n\n");
	v_Stored_Data[1] = v_Compass_Data[0];

	v_Compass_Data[0] = 0x0F;
	v_return_test = i2c_write(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Data, v_data_count);
	v_return_test = i2c_read(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Data, v_data_count);
	if (!v_return_test) printf("Error: Compass orientation setup\n\n");
	v_Stored_Data[2] = v_Compass_Data[0];

	v_Compass_Data[0] = 0x10;
	v_return_test = i2c_write(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Data, v_data_count);
	v_return_test = i2c_read(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Data, v_data_count);
	if (!v_return_test) printf("Error: Compass orientation setup\n\n");
	v_Stored_Data[3] = v_Compass_Data[0];

	v_Compass_Data[0] = 0x12;
	v_return_test = i2c_write(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Data, v_data_count);
	v_return_test = i2c_read(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Data, v_data_count);
	if (!v_return_test) printf("Error: Compass orientation setup\n\n");
	v_Stored_Data[4] = v_Compass_Data[0];

	v_Compass_Data[0] = 0x13;
	v_return_test = i2c_write(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Data, v_data_count);
	v_return_test = i2c_read(v_device, COMPASS_ADDRESS, v_Register_Address, v_Compass_Data, v_data_count);
	if (!v_return_test) printf("Error: Compass orientation setup\n\n");
	v_Stored_Data[5] = v_Compass_Data[0];
	
	for(i=0;i<6;i++)
	{
		strc_compass.previous_cal_data[i] = strc_compass.current_cal_data[i];
		strc_compass.current_cal_data[i] = v_Stored_Data[i];
	}
}

#endif /* _GUM  */


