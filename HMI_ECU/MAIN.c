/*
 *
 *
 *  Created on: Nov 2, 2023
 *  Author: Salma Mizar
 */
#include "main.h"
#include "LCD.h"
#include "keypad.h"
#include "UART.h"
#include "TIMER.h"
#include "std_types.h"
#include <avr/io.h>
#include <util/delay.h>

/* ========================================================
 * 					* NOTES*
 * 1. Using Switch-statment instead of if-statment give me some of speed and decrease CPU load
 * 2. Enter button -- '='
 * ========================================================*/

/************************************************************************
 * 						Global static variable used in ISR timer1
 **************************************************************************/
static volatile uint8 g_tick =0;


/*
 * Function Name: main()
 * Description : implementation of HMI ECU app
 */
int main(void)
{
	SREG|=(1<<7);					/* Enable i-bit*/
	/***********************************************
	 * 				UART Configurations
	 * 1.Even Parity
	 * 2.stop bit --onebit
	 * 3.8 bits Data
	 * 4.Baudrate= 9600
	 ***********************************************/
	UART_ConfigType configurations = {EIGHT_BIT, DISABLED, ONE_BIT, BaudRate7};
	UART_init( &configurations);

	/* Lcd initialization*/
	LCD_init();
	/* agreement(sink) between Two ECUs*/
	UART_sendByte(MC2_GET_READY);

	LCD_displayString("plz enter pass:");
	LCD_moveCursor(1,0);

	while(1)
	{
		/* Get first step in system */
		insertNewPassword();
	}

}
/*
 * Function Name: insertNewPassword()
 * Description : Take a new Pass from user and Check it
 */
void insertNewPassword(void)
{
	key = KEYPAD_getPressedKey();
	switch(key)
	{
	case 0:

	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		switch(numOfPass_flag)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			UART_sendByte(key);
			LCD_integerToString(key);
			_delay_ms(300);
			LCD_sendCommand(0x10);
			LCD_displayCharacter('*');
			numOfPass_flag ++;				/* Num of digits for password*/
			break;
		}

		break;
	case '=':
			/***************************************************
			 * 	Check that user insert pass from  five digits
			 ***************************************************/
			if(numOfPass_flag  != 0 && numOfPass_flag % 5 == 0 )
			{
				equality_flag++;			/* Increment num of '=' refer to enter button*/
				switch(equality_flag)
				{
				/* for enter first '='*/
				case 1:
					UART_sendByte(key);
					numOfPass_flag=0;
					LCD_sendCommand(0x01);
					LCD_displayStringRowColumn(0,0,"plz re-enter the");
					LCD_displayStringRowColumn(1,0,"same pass:");
					break;
					/*For enter second enter*/
				case 2:
					UART_sendByte(key);
					numOfPass_flag=0;
					equality_flag=-1;
					recieveCheckingPass();		 /* Function take checking from control_ECU*/
					break;
				}
			}

			break;
	}
	_delay_ms(300);  /* Press time */
}
/*
 * Function Name: recieveCheckingPass()
 * Description  : Recieve if pass is true or false from Control_ECU
 */

void recieveCheckingPass(void)
{
	/* agreement between two ECUs*/
	while(UART_recieveByte() != MC2_WILL_SEND_CHECK);
	/* Recieve checking of pass from another ECU*/
	switch(UART_recieveByte())
	{
	case PASS_IS_TRUE:
		/* Call function which starts running the system*/
		mainOptions();
		break;
		/*Case Two pass are not matched*/
	case PASS_IS_FALSE:
		/* Just display LCD*/
		LCD_sendCommand(0x01);
		LCD_displayStringRowColumn(0,2,"password are");
		LCD_displayStringRowColumn(1,2,"not matched :(");
		_delay_ms(300);
		LCD_sendCommand(0x01);
		LCD_displayStringRowColumn(0,0,"plz enter pass:");
		LCD_moveCursor(1,0);
		/* Re-call fisrt Step in System*/
		for(;;)
		{
			insertNewPassword();
		}
		break;
	}

}
/*
 * Function Name : mainOptions()
 * Description   : Choose which operation will be run
 */
void mainOptions(void)
{
	uint8 userkey;
	/* just display LCD*/
	LCD_sendCommand(0x01);
	LCD_displayStringRowColumn(0,0," + : Open Door");
	LCD_displayStringRowColumn(1,0," - : Change Pass");
	/* Confirm that user enter '+'  or '-' only*/
	while(userkey != '+' || userkey != '-')
	{
		userkey= KEYPAD_getPressedKey();
		switch(userkey)
		{
		case '+':
		case '-':
			UART_sendByte(userkey);
			/* Passing user option to global variable to use it when run the system*/
			userChoice = userkey;
			/* Call function That take pass from user*/
			getUserPass();
			break;
		}

	}
}
/*
 * Function Name: getUserPass()
 * Description  : get The user pass to run the system ^_^
 */
void getUserPass()
{
	uint8 userPass;
	uint8 digits=0;
	/* Agreement between two ECUs*/
	UART_sendByte(MC2_READY_TO_GET_USER_PASS);
	/* Just Display*/
	LCD_sendCommand(0x01);
	LCD_displayStringRowColumn(0,0,"plz enter pass:");
	LCD_moveCursor(1,0);
	/* Take Password until user push on enter "=" button*/
	while(userPass != '=')
	{
		userPass = KEYPAD_getPressedKey();
		switch(userPass)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			if(digits<5)
			{
				UART_sendByte(userPass);
				LCD_integerToString(userPass);
				_delay_ms(300);
				LCD_sendCommand(0x10);
				LCD_displayCharacter('*');
				digits++;
			}
			break;
		case '=' :
			if(digits  != 0 && digits % 5 == 0 )
			{
				UART_sendByte(userPass);
				/* Start Running the system*/
				systemRun();
			}
			else
			{
				userPass =' ';
			}
			break;
		}
		_delay_ms(300);  /* Press time */
	}

}
/*
 * Function Name : systemRun()
 * Description   : Recieve if pass equal the same pass in EEPROM and display operation depends on user choice
 */
void systemRun(void)
{
	/* Agreement between two ECUs*/
	UART_sendByte(CHECK_USER_PASS);
	/* Check if pass is true or not*/
	switch(UART_recieveByte())
	{
	case USER_PASS_IS_TRUE:
	{
		failedAttemp=0; /* Retrun failed attemps to zero */
		/* Check on user options*/
		switch(userChoice)
		{
		case '+':
			/* open Door display*/
			openDoorDisplay();
			/* re-turn to options*/
			mainOptions();
			break;
		case '-':
			/* Change Password*/
			LCD_sendCommand(0x01);
			LCD_displayStringRowColumn(0,0,"Enter New Pass:");
			LCD_moveCursor(1,0);
			/* Re-call to first step*/
			for(;;)
			{
				insertNewPassword();
			}
			break;
		}
	}
	break;
	/* Case user pass is false*/
	case USER_PASS_IS_FALSE:
	{
		/* increment failed attemps*/
		failedAttemp++;
		/* Check if failed attemps is three contious attemps*/
		switch(failedAttemp)
		{
		case 3:
			LCD_sendCommand(0x01);
			LCD_displayStringRowColumn(0,4,"ERROR !!");
			LCD_displayStringRowColumn(1,0,"Disabled System");
			myOwnDelay(60);
			_delay_ms(150);
			failedAttemp=0;
			mainOptions();
			break;
		}
		/* Just LCD display*/
		LCD_sendCommand(0x01);
		LCD_displayStringRowColumn(0,2,"Wrong Password");
		LCD_moveCursor(1,5);
		LCD_integerToString(failedAttemp);
		LCD_displayStringRowColumn(1,6,"/3");
		_delay_ms(500);
		/* return to take user pass*/
		getUserPass();
		break;
	}
	}
}
/*
 * Function Name : openDoorDisplay()
 * Description   : Display that door is unlocking
 */
void openDoorDisplay(void)
{
	/* Just display*/
	LCD_sendCommand(0x01);
	LCD_displayStringRowColumn(0,4,"Door is");
	LCD_displayStringRowColumn(1,4,"Unlocking  ");
	/* Delay from Timer1 , count seconds*/
	myOwnDelay(15);
	LCD_sendCommand(0x01);
	LCD_displayStringRowColumn(0,4,"Waiting ");
	/* Delay from Timer1 , count seconds*/
	myOwnDelay(3);
	LCD_sendCommand(0x01);
	LCD_displayStringRowColumn(0,4,"Door is");
	LCD_displayStringRowColumn(1,4,"Locking  ");
	/* Delay from Timer1 , count seconds*/
	myOwnDelay(15);
}

/*
 * Function Name : timerTicks()
 * Description: Just increment num. of ticks in timer 1s
 */
void timerTicks(void)
{
	g_tick++;
}
/*
 * Function Name : myOwnDelay()
 * Description   : initialize timer1 and count until to sec which is input from user
 */
void myOwnDelay(uint16 sec)
{
	g_tick=0;
	/***********************************************
	 * 				TIMER1 Configurations
	 * 1.Initial value
	 * 2.compare value(in ctc mode)
	 * 3.prescaler
	 * 4.mode of operation
	 ***********************************************/
	Timer1_ConfigType Timer1_Confige = { 0 ,0 , N_64 , Normal};
	Timer1_init(&Timer1_Confige);
	/* Used call back which call in ISR for time1*/
	Timer1_setCallBack(timerTicks);
	/* Delay*/
	while(g_tick < (2*sec));
	/* Deinitialization of timer1*/
	Timer1_deInit();

}

