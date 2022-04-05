/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.h
 *
 * Description: Header file for the Timer AVR driver
 *
 * Author: Menna Saeed
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"
#include <avr\interrupt.h>
#include "avr/io.h"
/*-----------------------------------------------------------------------------
 *                         Types Declaration                                   *
-------------------------------------------------------------------------------*/
typedef enum{
	TIMER_0_ID,TIMER_1_ID,TIMER_2_ID
}TIMER_ID;

typedef enum{
	NO_CLOCK,F_CPU_1,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}TIMER_Prescaler;

typedef enum{
	NORMAL_MODE,COMPARE_MODE
}TIMER_MODE;

/*-------------------------------------------------------------------------------
 * [Structure Name]: TIMER_ConfigType
 *
 * [Description]: This structure is responsible for maintaining information about the Timer configuration
 ----------------------------------------------------------------------------------*/
typedef struct
{
	/*
	 * Timer_ID is a struct member of type TIMER_ID
	 */
			TIMER_ID 		Timer_ID;
	/*
	 * prescaler is a struct member of type TIMER_Prescaler that indicates the selected clock
	 */
			TIMER_Prescaler prescaler;
	/*
	 * Timer_mode is a struct member of type TIMER_MODE that indicates the Timer selected mode
	 */
			TIMER_MODE		Timer_mode;
	/*
	 * Start_value is a uint16 variable to initiate the Timer Control Register
	 */
			uint16			Start_value;
	/*
	 * Compare_value is a uint16 variable used to set the Output compare register value in case of Compare mode selected
	 */
			uint16	   	    Compare_value;
}TIMER_ConfigType;

/*-------------------------------------------------------------------------------
 *                       Functions Prototypes                           		 *
--------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------
 * [Function Name]: Timer_setCallBack
 *
 * [Description]:  Function to set the Call Back function address.
 *
 * [Args]:        a_ptr: a void pointer to a void function that takes the address of the call back function
 *				  Timer_ID: a variable of type TIMER_ID that switches the call back function according to the passed Timer ID
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void Timer_setCallBack(void(*a_ptr)(void), TIMER_ID Timer_ID );
/*-------------------------------------------------------------------------------
 * [Function Name]: Timer_init
 *
 * [Description]:  Function to initialize the Timer
 *
 * [Args]:        Config_Ptr: a constant pointer to struct that defines the Timer required configuration
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void Timer_init (const TIMER_ConfigType * Config_Ptr);
/*-------------------------------------------------------------------------------
 * [Function Name]: Timer_DeInit
 *
 * [Description]:  Function to quit the Timer
 *
 * [Args]:        Timer_ID: a variable of type TIMER_ID that switches the call back function according to the passed Timer ID
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void Timer_DeInit(TIMER_ID Timer_ID);

#endif /* TIMER_H_ */
