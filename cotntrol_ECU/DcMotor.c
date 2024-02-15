/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: DcMotor.c
 *
 * Description: source file for the ATmega32 DcMotor driver
 *
 * Author: Salma Mizar
 *
 *******************************************************************************/
#include "gpio.h"
#include "DcMotor.h"
#include "PWM.h"

/***************************** Motor Functionality ********************************/
void motor_rotate_clockwise(void)
{
	GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN1, LOGIC_LOW);
	GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN2, LOGIC_HIGH);
}

void motor_rotate_anti_clockwise(void)
{
	GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN1, LOGIC_HIGH);
	GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN2, LOGIC_LOW);
}
void motor_stop(void)
{
	GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN1, LOGIC_LOW);
	GPIO_writePin(DcMotor_PORT_ID, DcMotor_PIN2, LOGIC_LOW);
}
 /*******************************************************************************/



void DcMotor_Init(void)
{
	/* set the DC motor pins as output pins*/
	GPIO_setupPinDirection(DcMotor_PORT_ID,  DcMotor_PIN1, PIN_OUTPUT);
	GPIO_setupPinDirection(DcMotor_PORT_ID,  DcMotor_PIN2, PIN_OUTPUT);

	/* stop the motor at the begging */

	motor_stop();
}

void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{

	PWM_Timer0_Start(speed);
	if (state == OFF)
	{
		 motor_stop();
	}

	else if (state == A_CW)
	{
		 motor_rotate_anti_clockwise();
	}
	else if (state == CW)
	{
		 motor_rotate_clockwise();


	}
}
