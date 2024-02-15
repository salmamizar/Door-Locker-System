/*
 * main.h
  *
 *  Created on: Nov 2, 2023
 *  Author: Salma Mizar
 */

#ifndef MAIN_H_
#define MAIN_H_
#include "std_types.h"

/*************************************************************************
 * 						Definitions
 *************************************************************************/
#define PASS_SIZE					 5
#define PASS_IS_TRUE				0x10
#define PASS_IS_FALSE				0x01
#define MC2_READY     				0xFF
#define MC2_READY_TO_GET_USER_PASS 	0xee
#define USER_PASS_IS_TRUE 			0x17
#define USER_PASS_IS_FALSE			0x16
#define  MC2_WILL_SEND_CHECK		0x20
#define  CHECK_USER_PASS    		0x55
/*************************************************************************
 * 						Function Prototypes
 *************************************************************************/
void checkOnPass(uint8 arr1 [], uint8 arr2[] ,uint8 arr3[] );
void myOwnDelay(uint16 sec);
void timerTicks(void);
void openDoor(void);
void systemOperation(void);
void writeEEPROM(uint8 arr4[]);
void checkOnEEPROMpass(uint8 array1[] , uint8 array2[]);
void activeBuzzzer(void);
void recievePass(uint8 arr[]);
void systemRun(uint8 userpass[] , uint8 newpass[] ,uint8 samepass[]);
/*************************************************************************
 * 						Flags
 *************************************************************************/
boolean isPassTrue = FALSE;

boolean isUserPassTrue = FALSE;

/* to know which operation will be run -- open door or change password */
uint8 SystemOperation;
/* To count number of failed*/
uint8 failedAttemp=0;


#endif /* MAIN_H_ */
