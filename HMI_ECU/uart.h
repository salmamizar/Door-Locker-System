 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"


typedef enum
{
	FIVE_BIT,SIX_BIT,SEVEN_BIT,EIGHT_BIT,NINE_BIT=0X07
}UART_BitData;

typedef enum
{
	DISABLED,RESERVED,EVEN,ODD
}UART_Parity;
typedef enum
{
	ONE_BIT,TWO_BIIT
}UART_StopBit;
typedef enum
{
	BaudRate1=10,
	BaudRate2=300,
	BaudRate3=600,
	BaudRate4=1200,
	BaudRate5=2400,
	BaudRate6=4800,
	BaudRate7=9600,
	BaudRate8=14400,
	BaudRate9=19200,
	BaudRate10=38400,
	BaudRate11=57600,
	BaudRate12=115200,
	BaudRate13=128000,
	BaudRate14=256000,
}UART_BaudRate;

typedef struct{
 UART_BitData bit_data;
 UART_Parity parity;
 UART_StopBit stop_bit;
 UART_BaudRate baud_rate;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
