/******************************************************************************
 *
 * Module: CTRL ECU main function
 *
 * File Name: Control.c
 *
 * Description: Door Locker Security System (Control ECU)
 *
 * Author: Menna Saeed
 *
 *******************************************************************************/

#include "CTRL_supportingFunctions.h"


int main ()
{
	SREG|=(1<<7); /*I-bit enable*/

	/*INITIALIZATION AND CONFIGURATION*/
	/*Uart Initialization */
	Uart_ConfigType UART_Configuration ={BIT_8,NO_PARITY,ONE_STOP_BIT,BAUD_RATE_9600};
	UART_init(&UART_Configuration);

	/*Timer Initialization */
	TIMER_ConfigType Timer_Configuration={TIMER_1_ID,F_CPU_1024,COMPARE_MODE,0,7813};
	Timer_init(&Timer_Configuration);
	Timer_setCallBack(Timer_callBack, TIMER_1_ID);

	/*TWI Initialization */
	TWI_ConfigType TWI_configuretion ={TWI_Prescaler_1,0x02,TWI_CONTROL_ECU_ADDRESS};
	TWI_init(&TWI_configuretion);

	/*DC motor initialization*/
	DcMotor_init();
	/*Buzzer initialization*/
	Buzzer_init();
	/*----------------------------------------------------------
	 *						 Control interface
		 ---------------------------------------------------------*/
	/*
	 * -First time to visit the program:
	 * 				The program receives 2 passwords, the initialized and the its confirmation
	 * 				and then it sends them to the CTRL ECU via UART to check if they are matched or not
	 * 				>In case they are matched: save them to the EEPROM in the CTRL ECU
	 * 				>In case they are not matched: ask the user to try again until matching occurs
	 */
	Setting_password_CTRL();
	/*
	 an array to store the password received from the HMI*/
	uint8 recieved_password[PASSWORD_LENGTH];

	/*
	 * variable used to count the number of Un-matching occurance
	 */
	uint8 num_wrong_pass_attemps=0;

	/*
	 * variable to store received option from the user interface
	 */
	uint8 screen_options=0;

	/*
	 * variable used to store the received byte via UART*/
	uint8 receivedByte=0;

	while(1){


		if (UART_recieveByte() == READY){

			Receive_password_from_HMI_ECU(recieved_password);

			/*
			 * receive option desired by user from HMI ECU
			 * >>>   '+' :  OPEN_DOOR_OPTION
			 * >>>	 '-' : CHANGE_PASSWORD_OPTION
			 */
			screen_options = UART_recieveByte();

			/* if the option is '+'	 */
			if (screen_options == OPEN_DOOR_OPTION){
				/*compare between the received password and the one stored to the EEPROM*/
				receivedByte=Compare_passwords(g_password,recieved_password);

				/*if the received password is matched with stored Password*/
				if (receivedByte == PASSOWRD_MATCH)
				{

					/*
					 * send an Opening door action to the HMI ECU for the passwords are matched
					 */
					UART_sendByte(Opening_Door_Action);

					/*
					 * reset the counter of wrong password received from HMI by user
					 * as if one matched password recieved will break the counter to start again from zero
					 */
					num_wrong_pass_attemps=0;

					/*
					 * starting door tasks:
					 * 		> open the door: 15 sec
					 * 		> then hold it for some time: 3 sec
					 * 		> then close it: 15 sec
					 */
					Door_tasksCTRL();

				}
				/*if the received password is Un-matched with the stored Password*/
				else if (receivedByte == PASSWORD_UNMATCH)
				{
					/*
					 * Increment the counter of wrong password received from HMI by user
					 */
					num_wrong_pass_attemps++;

					/*if the unmatched trials reach the max number of allowed wrong trial*/
					if (num_wrong_pass_attemps == NUMBER_OF_WRONG_PASSWORD_ATTEMPTS)
					{
						/*
						 * sending password state to HMI-ECU telling that the entered password by user
						 * 		is mismatched with the real password application for 3 times consecutively
						 * 		so the lCD in HMI-ECU show state of Danger
						 */
						UART_sendByte(Danger);

						/*
						 * start execution of Danger mission
						 * 		buzzer will work by high sound to show the danger state
						 */
						dangerMission();

						/*
						 * reset the counter of wrong password received from HMI by user
						 */
						num_wrong_pass_attemps=0;

					}
					else{
						/*
						 * sending password state to HMI-ECU telling that the entered password by user
						 * 		is mismatched with the real password application
						 */
						UART_sendByte(PASSWORD_UNMATCH);
					}
				}
			}
			/* if the option is '-'	 */
			else if (screen_options == CHANGE_PASSWORD_OPTION) {

				/*compare between the received password and the one stored to the EEPROM*/
				receivedByte=Compare_passwords(g_password, recieved_password);

				/*if the received password is matched with stored Password*/
				if ( receivedByte== PASSOWRD_MATCH) {
					/*
					 * sending to HMI-ECU that password matched and tell it to change password
					 */
					UART_sendByte(Changing_Password_Action);

					num_wrong_pass_attemps=0;

					Setting_password_CTRL();

				}
				/*if the received password is unmatched with stored Password*/
				else if(receivedByte==PASSWORD_UNMATCH){

					/*
					 * Increment the counter of wrong password received from HMI by user
					 * 		to use it in case repeated Mismatched password(may be thief)
					 */
					num_wrong_pass_attemps++;

					/*if the unmatched trials reach the max number of allowed wrong trial*/
					if (num_wrong_pass_attemps == NUMBER_OF_WRONG_PASSWORD_ATTEMPTS){

						/*
						 * sending password state to HMI-ECU telling that the entered password by user
						 * 		is mismatched with the real password application for 3 times consecutively
						 * 		so the lCD in HMI-ECU show state of Danger
						 */
						UART_sendByte(Danger);

						/*
						 * start execution of Danger mission
						 * 		buzzer will work by high sound to show the danger state
						 */
						dangerMission();

						/*
						 * reset the counter of wrong password received from HMI by user
						 */
						num_wrong_pass_attemps=0;

					}
					else
					{
						UART_sendByte(PASSWORD_UNMATCH);
					}
				}
			}
		}
	}

	return 0;

}



