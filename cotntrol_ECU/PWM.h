 /******************************************************************************
 *
 * Module: PWM
 *
 * File Name: PWM.h
 *
 * Description: header file for the ATmega32 PWM driver
 *
 * Author: Salma Mizar
 *
 *******************************************************************************/


#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
