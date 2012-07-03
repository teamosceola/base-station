// *********************************************************************************
// *****	Author:		Christopher Crews									********
// *****	Date:		10 February 2012									********
// *****	File Name: 	xbee.c												********
// *****																	********
// *****	The xbee.c file contains all the functions necessary to 		********
// *****	initialize, read, and calculate himidity.						********
// *********************************************************************************
// *****             Formated for Monospaced Type							********
#ifdef _GUM

// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include "xbee.h"
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Global Variables	 											********
// *********************************************************************************
xbee_struct strc_xbee;
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Xbee_Thread														********
// *****	Continuous thread that reads Xbee Sensor						********
// *****	return: void													********
// *****	(1) void pointer												********
// *********************************************************************************
void *Xbee_Thread(void *ptr)
{
// *********************************************************************************
// *****	Initialize Variables											********
	xbee_struct strc_temp_xbee;
	unsigned char v_temp_data[4];
	unsigned char v_string[500];
	int v_bytes_read;
	int v_EOS_flag = 0;
	int i = 0;
	int j = 0;
// ----------  END  ----------------------------------------------------------------
	
// *********************************************************************************
// *****	Main Loop of Xbee_Thread										********
	for(;;)
	{
		for(;;)
		{
			v_bytes_read = uart_read(v_uart_device,v_temp_data);
			for(i=0;i<v_bytes_read;i++)
			{
				strc_temp_xbee.v_rx_data[j+i] = v_temp_data[i];
				if(v_temp_data[i] == '\n')
				{
					v_EOS_flag = 1;
					break;
				}
			}
			j += i;
			if(v_EOS_flag == 1) break;
		}
		strc_temp_xbee.v_length = j;
		j = 0;
		v_EOS_flag = 0;
		strc_xbee = strc_temp_xbee;
		new_xbee_data = 1;
		usleep(100000);
	}
// ----------  END  Main Xbee_Thread Loop  -------------------------------------------
}
// ----------  END  Xbee_Thread  -----------------------------------------------------



// *********************************************************************************
// *****	Get_Xbee_Data													********
// *****	this function is used to get the data out of the 				********
// *****	Xbee.c file and back to the calling routine						********
// *****	(r) void														********
// ***** 	(1) xbee_struct *: pointer to a xbee_struct 					********
// *********************************************************************************
void Get_Xbee_Data(xbee_struct *temp)
{
	*temp = strc_xbee;		// ***** copy data into tmp struct
	new_xbee_data = 0;		// ***** set new data flag
}
// ----------  END GET_ALT_DATA  ---------------------------------------------------

// *********************************************************************************
// *****	Save_I2C_device_ID												********
// *****	this functions receives and saves the I2C device ID number		********
// *****	(r) void														********
// ***** 	(1) int: device ID									            ********
// *********************************************************************************
void save_xbee_uart_device_id(int UART_ID)
{
	v_uart_device = UART_ID;
	return;
}
// ----------  END SAVE_I2C_DEVICE_ID ----------------------------------------------

// ----------  END  ----------------------------------------------------------------
#endif /* _GUM  */


