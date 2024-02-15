/*
 * timer.h
 *
 *
 *  Created on: Nov 2, 2023
 *  Author: Salma Mizar
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                               types declaration                             *
 *******************************************************************************/
typedef enum{
	No_clk , N_1 , N_8 , N_64 , N_256 , N_1024 , External_Falling , External_Rising
}Timer1_Prescaler;

typedef enum{
	Normal , PWM_8bit ,PWM_9bit , PWM_10bit ,CTC_OCR1 ,FPWM_8bit ,FPWM_9bit ,FPWM_10bit
	,PWM_freq1 , PWM_freq2 ,PWM_phaseICR, PWM_phaseOCR1 ,CTC_ICR1 ,Reserved ,FPWM_ICR , FPWM1 ,FPWM2
}Timer1_Mode;

typedef struct{
	uint16 initial_value;
	uint16 compare_value;
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
}Timer1_ConfigType;

/***********************************************************************
 * 						Prototypes
 ***********************************************************************/
void Timer1_init(const Timer1_ConfigType *Config_ptr);
void Timer1_setCallBack(void(*a_ptr)(void));
void Timer1_deInit(void)
#endif /* TIMER_H_ */
