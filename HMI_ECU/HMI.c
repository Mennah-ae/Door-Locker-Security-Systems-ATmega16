/******************************************************************************
 *
 * File Name: App_HMI.c
 *
 * Description: Door Locker Security System (Human Machine Interface micro-controller)
 *
 * Author: Menna Saeed
 *
 *******************************************************************************/

#include "HMI_supportingFunctions.h"

int main (void)
{
	SREG|=(1<<7); /*I-bit enable*/

	/*INITIALIZATION AND CONFIGURATION*/
	/*LCD initialization*/
	LCD_init();

	/*Uart Initialization */
	Uart_ConfigType UART_Configuration ={BIT_8,NO_PARITY,ONE_STOP_BIT,BAUD_RATE_9600};
	UART_init(&UART_Configuration);

	/*Timer Initialization */
	TIMER_ConfigType Timer_Configuration={TIMER_1_ID,F_CPU_1024,COMPARE_MODE,0,7813};
	Timer_init(&Timer_Configuration);
	Timer_setCallBack(Timer_callBack, TIMER_1_ID);

	/*----------------------------------------------------------
	 *						 User interface
	 ---------------------------------------------------------*/
	/*
	 * -First time to visit the program:
	 * 				The program receives 2 passwords, the initialized and the its confirmation
	 * 				and then it sends them to the CTRL ECU via UART to check if they are matched or not
	 * 				>In case they are matched: save them to the EEPROM in the CTRL ECU
	 * 				>In case they are not matched: ask the user to try again until matching occurs
	 */
			Setting_password();

			/*a variable to get the pressed key from the keypad */
			uint8 Keypad_pressedKey=0;
			/*a variable to receive a special byte via UART that is common between the 2 ECUs */
			uint8 UART_stateReceived=0;
			/*an array to save the entered password  */
			uint8 Entered_password[PASSWORD_LENGTH];

	while(1)
	{
		HMI_mainOptions();
		/*
		 * will not exit the following loop until the user enters '+' or '-'
		 */
		do
		{
			Keypad_pressedKey = KEYPAD_getPressedKey();
		}while ( (Keypad_pressedKey != OPEN_DOOR_OPTION) && (Keypad_pressedKey != CHANGE_PASSWORD_OPTION) );

		/*in case the user entered '+': Open door option */
		if (Keypad_pressedKey == OPEN_DOOR_OPTION)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0,"Enter The Password");
			Get_password(Entered_password);
			UART_sendByte(READY); /* inform Control ECU to start sending */
			Send_passwordToControlECU(Entered_password);
			/* inform Control ECU the option that user chose */
			UART_sendByte(OPEN_DOOR_OPTION);
			/* Control ECU responses */
			UART_stateReceived = UART_recieveByte();

			if (UART_stateReceived == Opening_Door_Action)
			{
				Door_tasks(); /* start displaying door status on LCD */

			}
			else if (UART_stateReceived  == PASSWORD_UNMATCH)
			{
				unmatchedPasswordMSG();
			}
			HMI_mainOptions(); /* system back to idle & display main options */
		}
		/* in case the user entered '-': Change password option*/
		else if ( Keypad_pressedKey== CHANGE_PASSWORD_OPTION) {
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0,"Enter The Password");
			Get_password(Entered_password);
			UART_sendByte(READY); /* inform Control ECU to start sending */
			Send_passwordToControlECU(Entered_password);

			/* inform Control ECU the option that user chose */
			UART_sendByte(CHANGE_PASSWORD_OPTION);

			UART_stateReceived = UART_recieveByte();
			if (UART_stateReceived == Changing_Password_Action) {
				Setting_password();
			}
			else if (UART_stateReceived == PASSWORD_UNMATCH)
			{
				unmatchedPasswordMSG();
			}

		}
	}
	return 0;
}
