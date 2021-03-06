/******************************************************************************
 *
 * Module: Buzzer driver
 *
 * File Name: buzzer.h
 *
 * Description: Buzzer driver header file
 *
 * Author: Menna Saeed
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"

/*-------------------------------------------------------------------------------
 *                       Definitions                         					 *
--------------------------------------------------------------------------------*/
#define BUZZER_PORT_ID 	PORTC_ID
#define BUZZER_PIN_ID	PIN4_ID

/*-------------------------------------------------------------------------------
 *                       Functions Prototypes                          			 *
--------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------
 * [Function Name]: Buzzer_init
 *
 * [Description]:  Function to initialize the Buzzer
 *
 * [Args]:        void
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void Buzzer_init (void);

/*-------------------------------------------------------------------------------
 * [Function Name]: Buzzer_ON
 *
 * [Description]:  Function to turn the Buzzer on when danger mission executes
 *
 * [Args]:        void
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void Buzzer_ON (void);

/*-------------------------------------------------------------------------------
 * [Function Name]: Buzzer_OFF
 *
 * [Description]:  Function to turn the Buzzer off
 *
 * [Args]:        void
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void Buzzer_OFF (void);

#endif /* BUZZER_H_ */
