
/******************************************************************************
 *
 * Module: Buzzer driver
 *
 * File Name: buzzer.c
 *
 * Description: Buzzer driver source file
 *
 * Author: Menna Saeed
 *
 *******************************************************************************/

#include "buzzer.h"

/*-------------------------------------------------------------------------------
 *                       Functions Definitions                           		 *
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
void Buzzer_init (void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	Buzzer_OFF();
}
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
void Buzzer_ON (void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}
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
void Buzzer_OFF (void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}
