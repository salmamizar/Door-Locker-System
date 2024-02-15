 /******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: DcMotor.h
 *
 * Description: header file for the ATmega32 DcMotor driver
 *
 * Author: Salma Mizar
 *
 *******************************************************************************/
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DcMotor_PORT_ID 	PORTB_ID
#define DcMotor_PIN1 		PIN2_ID
#define DcMotor_PIN2 		PIN3_ID

/*******************************************************************************
 *                               types declaration                             *
 *******************************************************************************/
typedef enum
{
	OFF,CW,A_CW
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void DcMotor_Rotate(DcMotor_State state,uint8 speed);
void DcMotor_Init(void);
void motor_rotate_clockwise(void);
void motor_rotate_anti_clockwise(void);
void motor_stop(void);

#endif /* DC_MOTOR_H_ */
