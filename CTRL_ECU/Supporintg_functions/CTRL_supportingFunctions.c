
/******************************************************************************
 *
 * Module: Control ECU supporting functions
 *
 * File Name: CTR_supportingFunctions.c
 *
 * Description: Functions than are implemented in the CTRL ECU main function
 *
 * Author: Menna Saeed
 *
 *******************************************************************************/
#include "CTRL_supportingFunctions.h"

/*------------------------------------------------------------------------------
 *                              Global Variables                                *
--------------------------------------------------------------------------------*/
uint32 g_ticks;
uint8 g_password[PASSWORD_LENGTH];
/*--------------------------------------------------------------------------
 *                       Functions Definitions                            *
 ----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
 * [Function Name]: Receive_password_from_HMI_ECU
 *
 * [Description]:  Function that receives the password fro the HMI ECU via UART
 *
 * [Args]:         password: a pointer to uint8 data
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void Receive_password_from_HMI_ECU (uint8 * password)
{
	uint8 i;
	for (i=0;i<PASSWORD_LENGTH;i++)
	{
		password[i]=UART_recieveByte();
		_delay_ms(50);
	}
}
/*---------------------------------------------------------------------------
 * [Function Name]: Compare_passwords
 *
 * [Description]:  Function that compares the passwords received fron the HMI ECU
 *
 * [Args]:         password1,password2:  pointers to uint8 data
 *
 * [Returns]:      uint8 data: indicated the passwords status
 *
 ----------------------------------------------------------------------------------*/
uint8 Compare_passwords (uint8 *password1, uint8 *password2)
{
	uint8 i,tempCounter=0;
	for (i=0;i<PASSWORD_LENGTH;i++)
	{
		if (password1[i]==password2[i])
			tempCounter++;
	}
	if (tempCounter==PASSWORD_LENGTH)

		return PASSOWRD_MATCH;
	else
		return PASSWORD_UNMATCH;
}
/*---------------------------------------------------------------------------
 * [Function Name]: Save_passwordToEEPROM
 *
 * [Description]:  Function that saves the password in case they are matched
 *
 * [Args]:         password: a pointer to uint8 data
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void Save_passwordToEEPROM (uint8 *password)
{
	uint8 i;
	for(i=0;i<PASSWORD_LENGTH;i++)
	{
		EEPROM_writeByte(EEPROM_STORE_ADDREESS+i, password[i]);
		g_password[i]=password[i];
		_delay_ms(50);
	}
}

/*---------------------------------------------------------------------------
 * [Function Name]: Setting_password_CTRL
 *
 * [Description]:  Function that gets the passwords from the user
 *
 * [Args]:         void
 *
 * [Returns]:      Void
 *
----------------------------------------------------------------------------------*/
void Setting_password_CTRL (void)
{
	/*
	 *  FirstPassword: initialized password
	 *  SecondPassword: confirmation of password
	 */
	uint8 FirstPassword[PASSWORD_LENGTH];
	uint8 SecondPassword[PASSWORD_LENGTH];

	/*
	 * variable used to store the state of 2 passwords received by from HMI ECU
	 * can only be 2 cases:
	 * 						1- PASSWORD_MATCH
	 * 						2- PASSWORD_UNMATCH
	 */
	uint8 Password_State = PASSWORD_UNMATCH;

	while(Password_State == PASSWORD_UNMATCH)
	{
		/*communications between the 2 ECUs to check if they are ready to synchronize  */
		while (UART_recieveByte() != READY);
		UART_sendByte(READY);
		/*
		 * receive first password from HMI ECU  to compare it
		 *  with the confirmed password to check Matching
		 */
		Receive_password_from_HMI_ECU(FirstPassword);

		while (UART_recieveByte() != READY);
		UART_sendByte(READY);

		/*
		 * receive confirmed password from HMI ECU  to compare it
		 *  with the first password to check Matching
		 */
		Receive_password_from_HMI_ECU(SecondPassword);

		/*
		 * compare the two passwords to check the matching state
		 */
		Password_State = Compare_passwords(FirstPassword, SecondPassword);

		UART_sendByte(READY);

		UART_sendByte(Password_State);

		if ( Password_State == PASSOWRD_MATCH)
		{
			/*
			 * in case of matching: save to EEPROM
			 */
			Save_passwordToEEPROM(FirstPassword);
		}
	}
}
/*---------------------------------------------------------------------------
 * [Function Name]: Timer_callBack
 *
 * [Description]:  Function to be used by the Timer driver by incrementing the g_ticks
 *
 * [Args]:         void
 *
 * [Returns]:      Void
 *
----------------------------------------------------------------------------------*/

void Timer_callBack (void)
{
	g_ticks++;
}
/*-------------------------------------------------------------------------------
 * [Function Name]: Door_tasksCTRL
 *
 * [Description]:  Function to display door state
 *
 * [Args]:         void
 *
 * [Returns]:      Void
 *
----------------------------------------------------------------------------------*/
void Door_tasksCTRL(void)
{
	/*
	 * make g_ticks zero to start calculation and comparison of time
	 */
	g_ticks = 0;
	DcMotor_Rotate(CW);
	/*
	 * wait until door next state
	 */
	while (g_ticks < DOOR_OPENNING_TIME);

	g_ticks = 0;
	DcMotor_Rotate(STOP);
	/*
	 * wait until door next state
	 */
	while (g_ticks < DOOR_LEFT_OPEN_TIME);
	g_ticks = 0;
	DcMotor_Rotate(ACW);
	/*
	 * wait until door next state
	 */
	while (g_ticks < DOOR_CLOSING_TIME);

	DcMotor_Rotate(STOP);
}
/*-------------------------------------------------------------------------------
 * [Function Name]: dangerAlert
 *
 * [Description]:  Function to display a msg in case of un-matched passwords
 *
 * [Args]:         void
 *
 * [Returns]:      Void
 *
----------------------------------------------------------------------------------*/
void dangerMission(void)
{

	g_ticks=0;
	Buzzer_ON();
	/*
	 * wait until finishing danger time
	 */
	while(g_ticks < DANGER_TIME);
	Buzzer_OFF();
}
