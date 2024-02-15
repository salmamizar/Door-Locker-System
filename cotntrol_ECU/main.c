
/*
 *
 *  main.c
 *  Created on: Nov 2, 2023
 *  Author: Salma Mizar
 */

#include "main.h"
#include "External_EEPROM.h"
#include "TIMER.h"
#include "TWI.h"
#include "DCMOTOR.h"
#include "UART.h"
#include "Buzzer.h"
#include <util/delay.h>
#include <avr/io.h>
/* ========================================================
 * 					* NOTES*
 * 1. Using Switch instead of if give me some of speed and decrease CPU load
 * 2. Enter button -- '='
 * ========================================================*/
/*************************************************************************
 * 						Global Variable used in call back function
 *************************************************************************/
static volatile uint8 g_tick=0;
/*
 * Function Name : main()
 * Description   :implement Control ECU Code
 */
int main(void)
{
	/*Enable i-bit to use interrupt in TIMER1*/
	SREG |=(1<<7);
	/*save USER Password in global array*/
	uint8 userPass[PASS_SIZE];
	/* Save The Passwords in array*/
	uint8 newpass [PASS_SIZE], samePass[PASS_SIZE];
	/*******************************************************
	 * 				UART Configuration
	 * 				1.Even parity
	 * 				2.one stop Bit
	 * 				3. 8 Bits for data
	 * 				4.Baud Rate = 9600
	 *******************************************************/
	UART_ConfigType configurations = {EIGHT_BIT, DISABLED, ONE_BIT, BaudRate7};
	UART_init( &configurations);
	/*******************************************************
	 * 				I2C Configuration
	 * 				1.use 400k bit rate -- TWB=2
	 * 				2.Choice address number (4)
	 *******************************************************/
	TWI_ConfigType twi_configurations={0x02,0x04};
	TWI_init(&twi_configurations);
	/* DcMotor initialization*/
	DcMotor_Init();
	/* Buzzer initialization*/
	Buzzer_init();
	/* Send to MC1 that MC2 become Ready  to recieve orders*/
	while(UART_recieveByte() != MC2_READY);
	while(1)
	{
		/*Check if Pass and same pass are taken from HMI_ECU*/
		checkOnPass(newpass ,samePass ,userPass);
		/*Start runing the system*/
		systemRun(userPass,newpass,samePass);
	}


}
/*
 * Function Name : recievePass()
 * Description   : recieve password from HMI_ECU and save it in array
 */
void recievePass(uint8 arr[])
{
	uint8 i;
	for(i=0 ; i< PASS_SIZE ;i++)
	{
		arr[i]=UART_recieveByte();
	}
	/* recieve pass untill enter*/
	while(UART_recieveByte() != '=');
}

/*
 * Function Name:checkOnNewPass()
 * Description:
 * 	Recieve and Check if The two pass are the same or not , if are matched write it in EEPROM
 */
void checkOnPass(uint8 arr1 [], uint8 arr2[] ,uint8 arr3[] )
{
	uint8 i;
	recievePass(arr1);		/* recieve pass*/
	recievePass(arr2);		/* recieve confim pass*/
	/* Compare if two pass are matched or not*/
	for(i=0;i<PASS_SIZE;i++)
	{
		if(arr1[i] == arr2[i])
		{
			isPassTrue=TRUE;
		}
		else
		{
			isPassTrue=FALSE;
			break;
		}
	}
	/* Agreement between two ECUs*/
	UART_sendByte(MC2_WILL_SEND_CHECK);
	/* Send The result for HMI_ECU*/
	switch(isPassTrue)
	{
	case TRUE:
		UART_sendByte(PASS_IS_TRUE);	/* Send Result*/
		isPassTrue=FALSE;				/* Clear Flag*/
		writeEEPROM(arr1);				/* Write pass in EEPROM */
		systemOperation();				/*Recieve user option */
		/* Agreement between two ECUs*/
		while(UART_recieveByte() != MC2_READY_TO_GET_USER_PASS);
		/* Recieve User Pass*/
		recievePass(arr3);
		/* Check if equal the pass which write in EEPROM*/
		checkOnEEPROMpass(arr3 ,arr1);
		break;
		/* Case False*/
	case FALSE:
		UART_sendByte(PASS_IS_FALSE);	/*Send Result to HMI_ECU*/
		checkOnPass(arr1,arr2,arr3);	/* Re-call the function to do the same step*/
		break;
	}
}
/*
 * Function Name : checkOnEEPROMpass()
 * Description   : check on password in EEPROM
 */
void checkOnEEPROMpass(uint8 array1[] , uint8 array2[])
{
	uint8 i;
	/* First read pass which stored in EEPROM*/
	for(i=0 ; i<PASS_SIZE ;i++)
	{
		EEPROM_readByte((0x0C32)+i,array1+i);
	}
	/* Compare f two pass are the same or not , And Change Flag */
	for(i=0 ;i<PASS_SIZE ;i++)
	{
		if(array1[i]== array2[i])
		{
			isUserPassTrue=TRUE;		/* make flag true*/
		}
		else
		{
			isUserPassTrue=FALSE;		/* Make Flag False*/
			break;
		}
	}
}
/*
 * Function Name: systemRun()
 * Description  : Start running of the system
 */
void systemRun(uint8 userpass[] , uint8 newpass[] ,uint8 samepass[])
{
	/* To know if flag is true or false*/
	checkOnEEPROMpass(userpass ,newpass);
	/* Agreement between two ECUs*/
	while(UART_recieveByte ()!= CHECK_USER_PASS);
	/*Check on Flag of user password*/
	switch(isUserPassTrue)
	{
	case TRUE:

	{
		UART_sendByte(USER_PASS_IS_TRUE);	/* Send to HMI that the user pass is true*/
		failedAttemp=0;						/* Make failed Attemps =0*/
		/* Check on User choice*/
		switch(SystemOperation)
		{
		case '+' :
			openDoor();			/* turn on dcmotor , unlocking door*/
			/* return to know user option , Re-turn to step take operation from user*/
			systemOperation();
			/* Agreement between two ECUs*/
			while(UART_recieveByte() != MC2_READY_TO_GET_USER_PASS);
			/* take user pass*/
			recievePass(userpass);
			/* start running system again*/
			systemRun(userpass,newpass,samepass);
			break;
		case '-' :
			/* recieve change pass , And start Runnig system again*/
			checkOnPass(newpass,samepass,userpass);

			systemRun(userpass,newpass,samepass);
			break;
		}
	}
	break;
	/* Case False user pass*/
	case FALSE :
		UART_sendByte(USER_PASS_IS_FALSE);		/* Send Result to HMI_ECU*/
		/* increment failed attemps*/
		failedAttemp++;
		/* Check if failed attemps reach to three attemps continous*/
		switch(failedAttemp)
		{
		case 3:
			failedAttemp=0;		/* clear failed attemps*/
			/* Turn on Buzzer for 60 minute*/
			activeBuzzzer();
			/* Re-turb again to know which new operation that user need*/
			systemOperation();
			/* Agreement between two ECUs*/
			while(UART_recieveByte() != MC2_READY_TO_GET_USER_PASS);
			/* take user pass*/
			recievePass(userpass);
			/* start running system again*/
			systemRun(userpass,newpass,samepass);
			break;
		}
		/* Agreement between two ECUs*/
		while(UART_recieveByte() != MC2_READY_TO_GET_USER_PASS);
		/* take user pass*/
		recievePass(userpass);
		/* start running system again*/
		systemRun(userpass,newpass,samepass);
		break;
	}
}

/*
 * Function Name :systemOperation()
 * Description   : Know the operation from HMI_ECU
 */
void systemOperation(void)
{
	/* To save user options in Global variable to use it when starts the system*/
	SystemOperation=UART_recieveByte();
}
/*
 * Function Name : writeEEPROM()
 * Description   : Write in EEPROM in Assumption address
 */
void writeEEPROM(uint8 arr4[])
{
	uint8 k;
	/* Write pass in EEPROM*/
	for(k=0 ;k<PASS_SIZE ;k++)
	{
		EEPROM_writeByte((0x0C32)+k ,arr4[k]);
	}

}
/*
 * Function Name :openDoor()
 * Description   : Turn on Dc-Motor
 */
void openDoor(void)
{
	/* Turn on DC motor in Maximum speed in CW*/
	DcMotor_Rotate(CW ,255);
	/* count 15 sec from timer1*/
	myOwnDelay(15);
	/* turn off DcMotor*/
	DcMotor_Rotate(OFF ,0);
	/* count 3 sec fro timer 1*/
	myOwnDelay(3);
	/* Turn on DC motor in Maximum speed in CCW*/
	DcMotor_Rotate(A_CW ,255);
	/* count 15 sec from timer1*/
	myOwnDelay(15);
	/* turn off DcMotor*/
	DcMotor_Rotate(OFF ,0);
}
/*
 * Function Name : activeBuzzzer()
 * Description   : Active buzzer for 1 minute
 */
void activeBuzzzer(void)
{
	/* Start turn on Buzzer*/
	Buzzer_on();
	/* Count 60 sec from timer1*/
	myOwnDelay(60);
	/* Turn off Buzzer again*/
	Buzzer_off();
}
/*
 * Function Name : timerTicks()
 * Description   : increment number of ticks in timer1
 */
void timerTicks(void)
{
	g_tick++;
}
/*
 * Function Name : myOwnDelay()
 * Description   : delay till 2^16 sec maximum
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
	Timer1_ConfigType Timer1_Confige = {0, 0, N_64, Normal};
	Timer1_init(&Timer1_Confige);
	/* Used call back which call in ISR for time1*/
	Timer1_setCallBack(timerTicks);

	while(g_tick < (2*sec));
	Timer1_deInit();
}

