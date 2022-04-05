/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer_driver.c
 *
 * Description: Source file for the Timers AVR driver
 *
 * Author: Menna Saeed
 *
 *******************************************************************************/
#include "timer.h"
/*------------------------------------------------------------------------------
 *                              Global Variables                                *
--------------------------------------------------------------------------------*/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_Timer0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer1_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer2_callBackPtr)(void) = NULL_PTR;

/*------------------------------------------------------------------------------
 *                       Interrupt Service Routines                            *
-------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 *                              Timer0
------------------------------------------------------------------------------- */
/*
 * Timer/Counter0 Compare Match
 */
ISR(TIMER0_COMP_vect)
{
	if(g_Timer0_callBackPtr != NULL_PTR)
	{
		(*g_Timer0_callBackPtr)();
	}
}

/*
 * Timer/Counter0 Overflow
 */
ISR(TIMER0_OVF_vect)
{
	if(g_Timer0_callBackPtr != NULL_PTR)
	{
		(*g_Timer0_callBackPtr)();
	}
}
/*-------------------------------------------------------------------------
 *                              Timer1
 -------------------------------------------------------------------------*/
/*
 * Timer/Counter1 Overflow
 *
 */
ISR(TIMER1_OVF_vect)
{
	if(g_Timer1_callBackPtr != NULL_PTR)
	{
		(*g_Timer1_callBackPtr)();
	}
}

/*
 * Timer/Counter1 Compare Match A
 *
 */
ISR(TIMER1_COMPA_vect)
{
	if(g_Timer1_callBackPtr != NULL_PTR)
	{
		(*g_Timer1_callBackPtr)();
	}
}
/*------------------------------------------------------------------------
 *                              Timer2
 -----------------------------------------------------------------------*/
/*
 * Timer/Counter2 Compare Match
 *
 */
ISR(TIMER2_COMP_vect)
{
	if(g_Timer2_callBackPtr != NULL_PTR)
	{
		(*g_Timer2_callBackPtr)();
	}
}
/*
 * Timer/Counter2 Overflow
 *
 */
ISR(TIMER2_OVF_vect)
{
	if(g_Timer2_callBackPtr != NULL_PTR)
	{
		(*g_Timer2_callBackPtr)();
	}
}
/*-------------------------------------------------------------------------------
 *                       Functions Definitions                           		 *
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
void Timer_setCallBack(void(*a_ptr)(void), TIMER_ID Timer_ID )
{

	switch(Timer_ID)
	{
	case TIMER_0_ID:
		/* Save the address of the Call back function in a global variable */
		g_Timer0_callBackPtr = a_ptr;
		break;

	case TIMER_1_ID:
		/* Save the address of the Call back function in a global variable */
		g_Timer1_callBackPtr = a_ptr;
		break;

	case TIMER_2_ID:
		/* Save the address of the Call back function in a global variable */
		g_Timer2_callBackPtr = a_ptr;
		break;
	}

}
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
void Timer_init (const TIMER_ConfigType * Config_Ptr)
{
	/*------------------------------------------------------------------------------
	 *                              Timer0
	------------------------------------------------------------------------------- */
	if ((Config_Ptr->Timer_ID)==TIMER_0_ID)
	{
		TCNT0=Config_Ptr->Start_value;
		SET_BIT(TCCR0,FOC0); /*for non-pwm mode*/
		TCCR0=0;
		TCCR0=(TCCR0&0XF8)|(Config_Ptr->prescaler);

		if ((Config_Ptr->Timer_mode)==NORMAL_MODE)
		{
			SET_BIT(TIMSK,TOIE0); /*Enable overflow interrupt*/
			SET_BIT(TIFR,TOV0); /*Overflow flag*/
		}
		else if ((Config_Ptr->Timer_mode)==COMPARE_MODE)
		{
			SET_BIT(TCCR0,WGM01);
			OCR0=Config_Ptr->Compare_value;
			SET_BIT(TIMSK,OCIE0);
			SET_BIT(TIFR,OCF0);
		}
	}
	/*------------------------------------------------------------------------------
	 *                              Timer1
	------------------------------------------------------------------------------- */
	else if ((Config_Ptr->Timer_ID)==TIMER_1_ID)
	{
		TCCR1A=0;
		/*FOR NON-PWM MODE*/
		SET_BIT(TCCR1A,FOC1A);
		SET_BIT(TCCR1A,FOC1B);
		TCCR1B|=Config_Ptr->prescaler;
		TCNT1=Config_Ptr->Start_value;
		if ((Config_Ptr->Timer_mode)==NORMAL_MODE)
		{
			SET_BIT(TIMSK,TOIE1);
			SET_BIT(TIFR,TOV1);
		}
		else if ((Config_Ptr->Timer_mode)==COMPARE_MODE)
		{
			SET_BIT(TCCR1B,WGM12);
			OCR1A=Config_Ptr->Compare_value;
			SET_BIT(TIMSK,OCIE1A);
			SET_BIT(TIFR,OCF1A);
		}
	}
	/*------------------------------------------------------------------------------
	 *                              Timer2
	------------------------------------------------------------------------------- */
	else if ((Config_Ptr->Timer_ID)==TIMER_2_ID)
	{
		TCCR2=0;
		SET_BIT(TCCR2,FOC2);
		TCCR2|=(Config_Ptr->prescaler);
		TCNT2=Config_Ptr->Start_value;
		if ((Config_Ptr->Timer_mode)==NORMAL_MODE)
		{
			SET_BIT(TIMSK,TOIE2);
			SET_BIT(TIFR,TOV2);
		}
		else if ((Config_Ptr->Timer_mode)==COMPARE_MODE)
		{
			SET_BIT(TCCR2,WGM21);
			OCR2=Config_Ptr->Compare_value;
			SET_BIT(TIMSK,OCIE2);
			SET_BIT(TIFR,OCF2);
		}
	}
}
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
void Timer_DeInit(TIMER_ID Timer_ID)
{
	/*------------------------------------------------------------------------------
	 *                              Timer0
	------------------------------------------------------------------------------- */
	if (Timer_ID==TIMER_0_ID)
	{
		TCNT0=0;
		TCCR0=0;
		OCR0=0;
		CLEAR_BIT(TIMSK,OCIE0);
		CLEAR_BIT(TIMSK,TOIE0);
		g_Timer0_callBackPtr = NULL_PTR;
	}
	/*------------------------------------------------------------------------------
	 *                              Timer1
	------------------------------------------------------------------------------- */
	else if (Timer_ID==TIMER_1_ID)
	{
		TCCR1A=0;
		TCCR1B=0;
		TCNT1=0;
		OCR1A=0;
		CLEAR_BIT(TIMSK,OCIE1A);
		CLEAR_BIT(TIMSK,TOIE1);
		g_Timer1_callBackPtr = NULL_PTR;
	}
	/*------------------------------------------------------------------------------
	 *                              Timer2
	------------------------------------------------------------------------------- */
	else if (Timer_ID==TIMER_2_ID)
	{
		TCNT2=0;
		TCCR2=0;
		OCR2=0;
		CLEAR_BIT(TIMSK,TOIE2);
		CLEAR_BIT(TIMSK,OCIE2);
		g_Timer2_callBackPtr = NULL_PTR;
	}
}

