
/******************************************************************************
 *
 * Module: HMI ECU supporting functions
 *
 * File Name: HMI_supportingFunctions.c
 *
 * Description: Functions than are implemented in the HMI ECU main function
 *
 * Author: Menna Saeed
 *
 *******************************************************************************/

#include "HMI_supportingFunctions.h"

/*------------------------------------------------------------------------------
 *                              Global Variables                                *
--------------------------------------------------------------------------------*/
uint16 g_ticks = 0;
/*-------------------------------------------------------------------------------
 *                       Functions Definitions                           		 *
--------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------
 * [Function Name]: HMI_mainOptions
 *
 * [Description]:  Function to show the main options that the HMI ECU can display to the user
 * 					1- OPEN THE DOOR
 * 					2- CHANGE PASSWORD
 *
 * [Args]:         Void
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void HMI_mainOptions (void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0,"+: Open Door");
	LCD_displayStringRowColumn(1, 0, "-: Change Password");
}
/*-------------------------------------------------------------------------------
 * [Function Name]: Get_password
 *
 * [Description]:  Function to get the password from the user via KEYPAD
 *
 * [Args]:         password: a pointer to uint8
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void Get_password (uint8 *password)
{
	LCD_moveCursor(1, 0);
	/*variable used to get the keypad button pressed by user */
	uint8 key,i;

	for(i=0;i<PASSWORD_LENGTH;i++)
	{
		key = KEYPAD_getPressedKey();
		/*password must be numbers only*/
		if (key >= 0 && key <= 9) {
			LCD_displayCharacter('*');
			password[i]= key;
		}else
		{
			/*in case of pressing not a number button
			 * i will decrement the i counter to get 5 number password only
			 */
			i--;
		}
		_delay_ms(KEYPAD_PRESSED_TIME);
	}
	/*
	 * do not exit from the function without pressing a ENTER button
	 */
	while(KEYPAD_getPressedKey() != ENTER_ASCII);
}
/*-------------------------------------------------------------------------------
 * [Function Name]: Send_passwordToControlECU
 *
 * [Description]:  Function to send the password to the Control ECU via UART
 *
 * [Args]:         pass: a pointer to uint8
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/

void Send_passwordToControlECU(uint8 *pass)
{
	for (uint8 i=0;i<PASSWORD_LENGTH;i++)
	{
		UART_sendByte(pass[i]);
		_delay_ms(50);
	}
}
/*-------------------------------------------------------------------------------
 * [Function Name]: Setting_password
 *
 * [Description]:  Function to change the password
 *
 * [Args]:         void
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void Setting_password (void)
{
	uint8 Password_entered[PASSWORD_LENGTH]; /*for initialized password*/
	uint8 Password_confirmed[PASSWORD_LENGTH]; /*for password confirmation*/
	uint8 Password_state=PASSWORD_UNMATCH; /*to store password state*/

	while (Password_state==PASSWORD_UNMATCH)
	{
		LCD_clearScreen();
		LCD_displayString("Enter New Pass");

		Get_password(Password_entered);
		UART_sendByte(READY);
		while(UART_recieveByte()!=READY);
		Send_passwordToControlECU(Password_entered);
		/*To get confirmation from user*/
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Confirm Password");

		Get_password(Password_confirmed);
		UART_sendByte(READY);
		while(UART_recieveByte()!=READY);
		Send_passwordToControlECU(Password_confirmed);
		while(UART_recieveByte()!=READY);
		Password_state=UART_recieveByte();
		if (Password_state==PASSOWRD_MATCH)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Password Match");
			LCD_displayStringRowColumn(1, 0, "Password Saved!");
			_delay_ms(500);
		}
		else if (Password_state==PASSWORD_UNMATCH)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Password Un-match");
			LCD_displayStringRowColumn(1, 0, "Try again!");
			_delay_ms(500);
		}
	}
	LCD_clearScreen();
}
/*-------------------------------------------------------------------------------
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
 * [Function Name]: Door_tasks
 *
 * [Description]:  Function to display door state
 *
 * [Args]:         void
 *
 * [Returns]:      Void
 *
----------------------------------------------------------------------------------*/
void Door_tasks (void)
{
	g_ticks=0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is opening...");
	/*
	 * waiting until the door state changed
	 */
	while(g_ticks<DOOR_OPENNING_TIME);


	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is open");
	while(g_ticks<DOOR_LEFT_OPEN_TIME);


	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is locking.. ");
	while(g_ticks<DOOR_CLOSING_TIME);
}
/*-------------------------------------------------------------------------------
 * [Function Name]: unmatchedPasswordMSG
 *
 * [Description]:  Function to display a msg in case of un-matched passwords
 *
 * [Args]:         void
 *
 * [Returns]:      Void
 *
----------------------------------------------------------------------------------*/
void unmatchedPasswordMSG(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Un-matched Password!");
	LCD_displayStringRowColumn(1, 0, "Try again...");
	_delay_ms(500);
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
void dangerAlert(void)
{
	g_ticks=0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "DANGER !");
	LCD_displayStringRowColumn(1,0,"ALERT ON!");

	while(g_ticks < DANGER_TIME ){};

}

