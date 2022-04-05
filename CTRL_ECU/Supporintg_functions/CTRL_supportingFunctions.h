/******************************************************************************
 *
 * Module: Control ECU supporting functions
 *
 * File Name: CTRL_supportingFunctions.h
 *
 * Description: Functions than are implemented in the Control ECU main function
 *
 * Author: Menna Saeed
 *
 *******************************************************************************/
#ifndef CTRL_SUPPORTINGFUNCTIONS_H_
#define CTRL_SUPPORTINGFUNCTIONS_H_

#include "std_types.h"
#include "uart.h"
#include "util/delay.h"
#include "avr/io.h"
#include "twi.h"
#include "external_eeprom.h"
#include "dc_motor.h"
#include "buzzer.h"
#include "timer.h"
/*------------------------------------------------------------------------------
 *                              Definitions                                 	*
--------------------------------------------------------------------------------*/
/*Password related definitions*/
#define PASSWORD_LENGTH  					 5
#define PASSOWRD_MATCH    					 1
#define PASSWORD_UNMATCH  					 0
#define ENTER_ASCII		 				    13
#define NUMBER_OF_WRONG_PASSWORD_ATTEMPTS 	 3
/*To communicate with Control ECU*/
#define Opening_Door_Action				    0x88
#define Changing_Password_Action		    0x44
#define READY 								0x10
#define Danger 								0x33
/*Door options*/
#define OPEN_DOOR_OPTION					'+'
#define CHANGE_PASSWORD_OPTION				'-'

/*Timing*/
#define DOOR_OPENNING_TIME				    1788 //15sec
#define DOOR_CLOSING_TIME				    1788 //15sec
#define DOOR_LEFT_OPEN_TIME	 			    357 //3sec
#define KEYPAD_PRESSED_TIME				    500
#define DANGER_TIME						 	7152 //60sec
#define EEPROM_STORE_ADDREESS		   	    0x00

/*------------------------------------------------------------------------------
 *                              Global Variables                                *
--------------------------------------------------------------------------------*/
extern uint32 g_ticks;
extern uint8 g_password[PASSWORD_LENGTH];
/*--------------------------------------------------------------------------
 *                       Functions Prototypes                            *
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
void Receive_password_from_HMI_ECU (uint8 * password);
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
uint8 Compare_passwords (uint8 *password1, uint8 *password2);
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
void Save_passwordToEEPROM (uint8 *password);
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
void Setting_password_CTRL (void);
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

void Timer_callBack (void);
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
void Door_tasksCTRL(void);
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
void dangerMission(void);

#endif /* CTRL_SUPPORTINGFUNCTIONS_H_ */
