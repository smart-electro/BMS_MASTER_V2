/**********************************************************************/
// Battery Management System (c) by WWW.SMART-ELECTRO.NET
// This source code is subject to the CreativeCommons Attribution 4.0 International (CC BY 4.0) license
//
// You can copy and redistribute the material in any medium or format.
// Remix, transform, and build upon the material for any purpose, even commercially as long as you
// don't forget to mentioned www.smart-electro.net in your work.
// More about license can be found at: http://http://creativecommons.org/licenses/by/4.0/deed.en
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LporIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Version: V2.0
// Author: Tadej Gortnar <tadej.gortnar@gmail.com>
// Created: Dec 2010
//
// Modified: Feb 2014 Simon.grabar <simon.grabar@gmail.com>
// -Updated header and comments
// -Updated to support 32 slaves
//
// Application specific variable declarations

/* SETTINGS SECTION     */
/**********************************************************/
//numer of master unit. The number will be in CAN adress
#define MASTER_NUMBER 2;
/**********************************************************/
#include "j1939_data.h"
#include "can18xx8.h"

//config bits (fuses)
/*******************************************************/
//#pragma config BOREN = SBORDIS
#pragma config BOREN = OFF
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BORV = 0         // Brown-out Reset Voltage bits (Maximum Setting)

#pragma config WDT = ON        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)



#pragma config STVREN = OFF 
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)

#define NODE_EE_ADDRES 0;
#define BAT_EE_NUMBER 8;

#define set_bit(ADDRESS,BIT) (ADDRESS |= (1L<<BIT))
#define clear_bit(ADDRESS,BIT) (ADDRESS &= ~(1L<<BIT))
#define toggle_bit(ADDRESS,BIT) (ADDRESS ^= (1L<<BIT))
#define test_bit(ADDRESS,BIT) (ADDRESS & (1L<<BIT))


/*******************************************************/



// Function definitions
/*******************************************************/
J1939_MESSAGE tx_CAN_msg, rx_CAN_msg;
J1939_SE_ID Can_addrLO, Can_addrHI;
J1939_SE_ID Can_bootF, Can_nodeF, Can_Mask1, Can_Mask2; 

//when byte is received this function orders it and calculate its payload
void processReceivedByte(char Rx_Char);

int hexCharToInt(BYTE digit);
int hexToInt(BYTE *h);

void SestaviTxStr();
void SestaviCAN1();
void SestaviCAN2();
void NaslednjiPort();

void main (void);
void InterruptHandlerHigh (void);
void InterruptHandlerLow (void);


// CAN module related variables
enum CAN_RX_MSG_FLAGS RecFlags;

// Application specific initialization code follows
BYTE V_BatSerNo[8];                              // Battery (slave) number
BYTE V_BatNo;                                    // Stevilka baterije iz katere beremo podatke
BYTE V_BatChar[8];                               // Meritev iz baterije
int V_BatTemp;                                   // Vrednost ki smo jo dobili iz baterije
unsigned long cellStatuses[10];                  // bug in compiler!!!! we dont need array!
BYTE V_BatL[6];                                 // Rezultati meritev vseh 8 beterij
BYTE V_BatH[6];                                 // Rezultati meritev vseh 8 beterij
char Bat_Tx_Str[40];


BYTE RxFilterMatch;
BYTE rx_cnt, tx_cnt;
int rx_tic;
//tx_send,
