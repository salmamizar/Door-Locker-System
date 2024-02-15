/*
 * buzzer.h
  *
 *  Created on: Nov 2, 2023
 *  Author: Salma Mizar
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"

#define BUZZER_PORT_ID PORTB_ID
#define BUZZER_PIN_ID PIN5_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void Buzzer_init();
/* Description
 Setup the direction for the buzzer pin as output pin through the GPIO driver.
Turn off the buzzer through the GPIO.
 Inputs: None
Return: None
*/
void Buzzer_on(void);
/*Description
Function to enable the Buzzer through the GPIO.
 Inputs: None
 Return: None
 */
 void Buzzer_off(void);
 /*
 Description
 Function to disable the Buzzer through the GPIO.
 Inputs: None
 Return: None
 */

#endif /* BUZZER_H_ */
