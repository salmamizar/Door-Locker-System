/*
 * timer.c
 *
 *
 *  Created on: Nov 2, 2023
 *  Author: Salma Mizar
 */


#include "TIMER.h"
#include <avr/interrupt.h>		/*For use Interrupt of AVR*/
#include <avr/io.h>				/*For use Timer1 Register*/

/********************************************************
 * Global pointer to function
 * 1.static to use in this file and hold his value
 * 2.volatile to use in ISRs function
 * 3.Initial value with Null or use NULL_Ptr in standard_types
 ********************************************************/
static volatile void (*g_ptr)(void)= NULL_PTR;

/***********************************************************
 * 				Interrupt Service Routine for Compare Mode
 ************************************************************/
ISR(TIMER1_COMPA_vect)
{
	if(g_ptr != NULL_PTR)
	{
		g_ptr(); /* Call Back for function which i want to use it in ISR by pointer to function*/
	}
}
/***********************************************************
 * 				Interrupt Service Routine for Normal Mode
 ************************************************************/
ISR(TIMER1_OVF_vect)
{
	if(g_ptr != NULL_PTR)
	{
		g_ptr(); /* Call Back for function which i want to use it in ISR by pointer to function*/
	}
}
/*
 * Function Name: Timer1_init
 *	Description: Initialize Timer1 and select mode and prescaler
 */
void Timer1_init(const Timer1_ConfigType *Config_ptr)
{
	/*Initial value of Timer1*/
	TCNT1= Config_ptr->initial_value;
	TCCR1B= (TCCR1B & 0xF8) | (Config_ptr->prescaler);

	if(Config_ptr->mode == Normal)
	{
		TCCR1A|= (1<<FOC1A) | (1<<FOC1B);
		TIMSK |= (1<<TOIE1);

	}
	else if(Config_ptr->mode == CTC_OCR1 )
	{
		TCCR1A= (1<<FOC1A) | (1<<FOC1B);
		TCCR1B |= (1<<WGM12);
		OCR1A= Config_ptr->compare_value;
		TIMSK |= (1<< OCIE1A);
	}
	else if(Config_ptr->mode == CTC_ICR1)
	{
		TCCR1A= (1<<FOC1A) | (1<<FOC1B);
		TCCR1B = (1<<WGM12) | (1<<WGM13);
		ICR1= Config_ptr->compare_value;
		TIMSK |= (1<< OCIE1A);
	}
}
/*
 * Function Name: Timer1_setCallBack
 * Description: Call The function that i will implement it in ISR by save it's address in  global pointer to function
 */
void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_ptr=a_ptr;
}
/*
 * Function Name: Timer1_deIinit
 * Description: de-activation Timer1
 */
void Timer1_deInit(void)
{
	TCNT1=0;
	TCCR1A=0;
	TCCR1B=0;
	OCR1A=0;
	TIMSK=0;

}

