// *********************************************************************************
// *****	Author:		Christopher Crews									********
// *****	Date:		10 February 2012									********
// *****	File Name: 	accel.c												********
// *****																	********
// *****	The alt.c file contains all the functions necessary to 			********
// *****	initialize, read, and calculate accel.							********
// *********************************************************************************
// *****             Formated for Monospaced Type							********
#ifdef _GUM

// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include "accel.h"
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Global Variables	 											********
// *********************************************************************************
accel_struct strc_accel;
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Accel_Thread													********
// *****	Continuous thread that reads Accel Sensor						********
// *****	return: void													********
// *****	(1) void pointer												********
// *********************************************************************************
void *Accel_Thread(void *ptr)
{
// *********************************************************************************
// *****	Initialize Variables											********
	accel_struct strc_temp_accel;
// ----------  END  ----------------------------------------------------------------

	unsigned char v_data[3];
	int v_data_count;
	v_data_count = 1;
	v_data[0] = 0x47;
	i2c_write(v_device,ACCEL_ADDRESS,0x20,v_data,v_data_count);

	
// *********************************************************************************
// *****	Main Loop of Accel_Thread										********
	for(;;)
	{
		Read_Accel(&strc_temp_accel);
		strc_accel = strc_temp_accel;
		new_accel_data = 1;
		usleep(500000);
	}
// ----------  END  Main Accel_Thread Loop  ----------------------------------------
}
// ----------  END  Accel_Thread  --------------------------------------------------



// *********************************************************************************
// *****	Get_Accel_Data													********
// *****	this function is used to get the data out of the 				********
// *****	accel.c file and back to the calling routine					********
// *****	(r) void														********
// ***** 	(1) accel_struct *: pointer to a accel_struct 					********
// *********************************************************************************
void Get_Accel_Data(accel_struct *temp)
{
	*temp = strc_accel;		// ***** copy data into tmp struct
	new_accel_data = 0;		// ***** set new data flag
}
// ----------  END GET_ALT_DATA  ---------------------------------------------------

// *********************************************************************************
// *****	Save_I2C_device_ID												********
// *****	this functions receives and saves the I2C device ID number		********
// *****	(r) void														********
// ***** 	(1) int: device ID									            ********
// *********************************************************************************
void save_accel_i2c_device_id(int I2C_ID)
{
	v_device = I2C_ID;
	return;
}
// ----------  END SAVE_I2C_DEVICE_ID ----------------------------------------------

// *********************************************************************************
// *****	Read_Accel														********
// *****	This functions reads the accel									********
// *****	(r) void														********
// ***** 	(1) accel_struct *: pointer to accel structure					********
// *********************************************************************************
void Read_Accel(accel_struct *temp)
{
	unsigned char v_data[3];
	int v_data_count;
	v_data_count = 0;
	i2c_write(v_device,ACCEL_ADDRESS,X_REG,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ACCEL_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_Ax = v_data[0];
	v_data_count = 0;
	i2c_write(v_device,ACCEL_ADDRESS,Y_REG,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ACCEL_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_Ay = v_data[0];
	v_data_count = 0;
	i2c_write(v_device,ACCEL_ADDRESS,Z_REG,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ACCEL_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_Az = v_data[0];
	return;
}
// ----------  END  ----------------------------------------------------------------

// ----------  END  ----------------------------------------------------------------
#endif /* _GUM  */



