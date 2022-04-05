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
#ifndef HMI_SUPPORTINGFUNCTIONS_H_
#define HMI_SUPPORTINGFUNCTIONS_H_

#include "keypad.h"
#include "util/delay.h"
#include "uart.h"
#include "avr/io.h"
#include "timer.h"
#include "std_types.h"
#include "lcd.h"
#include <avr/interrupt.h>
/*------------------------------------------------------------------------------
 *                              Definitions                                 	*
--------------------------------------------------------------------------------*/
/*Password related definitions*/
#define PASSWORD_LENGTH  					 5
#define PASSOWRD_MATCH   					 1
#define PASSWORD_UNMATCH  					 0
#define ENTER_ASCII						     13
#define NUMBER_OF_WRONG_PASSWORD_ATTEMPTS  	 3
/*To communicate with Control ECU*/
#define Opening_Door_Action 				0x88
#define Changing_Password_Action 			0x44
#define READY 								0x10
#define Danger 								0x33
/*Door options*/
#define OPEN_DOOR_OPTION					'+'
#define CHANGE_PASSWORD_OPTION				'-'

/*Timing*/
#define DOOR_OPENNING_TIME				  1788 //15sec
#define DOOR_CLOSING_TIME				  1788 //15sec
#define DOOR_LEFT_OPEN_TIME	 			  357 //3sec
#define KEYPAD_PRESSED_TIME 			  500
#define DANGER_TIME						 7152 //60sec
/*------------------------------------------------------------------------------
 *                              Global Variables                                *
--------------------------------------------------------------------------------*/
extern uint16 g_ticks;
/*-------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------
 *                       Functions Prototypes                               *
----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
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
void HMI_mainOptions (void);

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
void Get_password (uint8 *password);
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

void Send_passwordToControlECU(uint8 *pass);
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
void Setting_password (void);
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
void Timer_callBack (void);
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
void Door_tasks (void);
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
void unmatchedPasswordMSG(void);
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
void dangerAlert(void);
#endif /* HMI_SUPPORTINGFUNCTIONS_H_ */
