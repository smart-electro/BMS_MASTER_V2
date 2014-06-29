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
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
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
#include <p18cxxx.h>
#include <stdio.h>
#include <string.h>
#include <timers.h> 
#include <usart.h>
#include <eep.h>
#include <spi.h>

#include "can18xx8.h"
#include "j1939_data.h"


/*******************************************************/
// bits in variable represent connectors on pcb board. 
// Only connectors(bits) that are set will be read.
// bits are in reverse order!!
//unsigned long connectedCells=0b11011101110111011101110111011101; //simon
//unsigned long connectedCells=0b00111011101110111011101110111011; //Uros No1 23cells
unsigned long connectedCells=0b10111111101110111011101111111111; //Uros No2 27cells



//numer of master unit. The number will be in CAN adress
#define MASTER_NUMBER 2;
/*******************************************************/

//config bits (fuses)
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


int hexCharToInt(BYTE digit);
int hexToInt(BYTE *h);

void ReadBatNo();

void SestaviTxStr();
void SestaviCAN1();
void SestaviCAN2();
void NaslednjiPort();

void main (void);
void InterruptHandlerHigh (void);
void InterruptHandlerLow (void);

J1939_MESSAGE tx_CAN_msg, rx_CAN_msg;
J1939_SE_ID Can_addrLO, Can_addrHI;
J1939_SE_ID Can_bootF, Can_nodeF, Can_Mask1, Can_Mask2; 

// CAN module related variables
enum CAN_RX_MSG_FLAGS RecFlags;

// Application specific initialization code follows
BYTE V_BatSerNo[8];                              // Battery (slave) number
BYTE V_BatNo;                                    // Stevilka baterije iz katere beremo podatke
BYTE V_BatChar[8];                               // Meritev iz baterije
int V_BatTemp;                                   // Vrednost ki smo jo dobili iz baterije
unsigned long cellStatuses[10];                  // bug in compiler!!!! we dont need array!
BYTE V_BatL[32];                                 // Rezultati meritev vseh 8 beterij
BYTE V_BatH[32];                                 // Rezultati meritev vseh 8 beterij
char Bat_Tx_Str[40];
BYTE Rx_Char;

BYTE RxFilterMatch;
BYTE rx_cnt, tx_cnt;
BYTE tx_send, rx_tic;

BYTE node_no;



/*********************************************************/
/*              auxiliary functions                      */
/*********************************************************/
int hexCharToInt(BYTE digit)
{
	if('0'<=digit && digit<='9')
		return digit-'0';
	if('A'<=digit && digit<='F')
		return digit-'A'+10;
	if('a'<=digit && digit<='f')
		return digit-'a'+10;
	return -1;
}

int hexToInt(BYTE *h)
{
	int i, result=0;
	int tmp;
	
	for(i=0;h[i] && i<8;i++)
    {
		tmp = hexCharToInt(h[i]);
		if(tmp == -1) return result;
		result = result*0x10+tmp;
	}
	return result;
}

// read number of batteryes from EEPROM 
void ReadBatNo()
{
    BYTE i;
	unsigned int a = BAT_EE_NUMBER;
	
    
	for(i=0;i<8;i++)
    {
        V_BatSerNo[i] = Read_b_eep(i + a);
    }
}
//create text string
/*void SestaviTxStr()
{
    BYTE i,s = 0;
    
    s += sprintf(Bat_Tx_Str +s,"M:");
    for(i=0;i<8;i++)
    {
        s += sprintf(Bat_Tx_Str +s,"%03X,",V_Bat[i]);
    }
    //Bat_Tx_Str[s] = '\r\0';
	Bat_Tx_Str[s] = '\r';
}*/

// create CAN massage
/*void SestaviCAN1()
{
    tx_CAN_msg.Data[0] = ((V_Bat[0] >> 8) & 0x03) +(V_BatSerNo[0] << 2);
    tx_CAN_msg.Data[1] = (V_Bat[0] & 0xFF);
    tx_CAN_msg.Data[2] = ((V_Bat[1] >> 8) & 0x03) +(V_BatSerNo[1] << 2);
    tx_CAN_msg.Data[3] = (V_Bat[1] & 0xFF);
    tx_CAN_msg.Data[4] = ((V_Bat[2] >> 8) & 0x03) +(V_BatSerNo[2] << 2);
    tx_CAN_msg.Data[5] = (V_Bat[2] & 0xFF);
    tx_CAN_msg.Data[6] = ((V_Bat[3] >> 8) & 0x03) +(V_BatSerNo[3] << 2);
    tx_CAN_msg.Data[7] = (V_Bat[3] & 0xFF);
    tx_CAN_msg.Address = Can_addrLO;
    tx_CAN_msg.Length.Len = 8;
}

void SestaviCAN2()
{
    tx_CAN_msg.Data[0] = ((V_Bat[4] >> 8) & 0x03) +(V_BatSerNo[4] << 2);
    tx_CAN_msg.Data[1] = (V_Bat[4] & 0xFF);
    tx_CAN_msg.Data[2] = ((V_Bat[5] >> 8) & 0x03) +(V_BatSerNo[5] << 2);
    tx_CAN_msg.Data[3] = (V_Bat[5] & 0xFF);
    tx_CAN_msg.Data[4] = ((V_Bat[6] >> 8) & 0x03) +(V_BatSerNo[6] << 2);
    tx_CAN_msg.Data[5] = (V_Bat[6] & 0xFF);
    tx_CAN_msg.Data[6] = ((V_Bat[7] >> 8) & 0x03) +(V_BatSerNo[7] << 2);
    tx_CAN_msg.Data[7] = (V_Bat[7] & 0xFF);
    tx_CAN_msg.Address = Can_addrHI;
    tx_CAN_msg.Length.Len = 8;
}
*/
//next port - cell for 8 cel master option
/*void NaslednjiPort()
{
    LATDbits.LATD3 = 1;
    V_BatNo++;                      // in gremo na novo baterijo
    rx_cnt = 0;
    LATD = ((LATD & 0xF0) | (V_BatNo & 0x07));
	//LATDbits.LATD3 = 0;

    if (V_BatNo == 4)              // Ali imamo prvo polovico podatkov iz baterij
    {
        if ( CANIsTxReady() )      // Sestavi CAN telegram 1 za MasterA.
        {
            SestaviCAN1();         // Sestavi podatke za CAN telegram
            CANSendMessage(tx_CAN_msg.Address.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
        } 
    }
    if (V_BatNo >= 8)              // Ali imamo drugo polovico podatkov iz baterij
    {
        memset(Bat_Tx_Str,'\0',sizeof(Bat_Tx_Str)); // Bruši USART buffer
        SestaviTxStr();            // Saestavi USART telegram za vse baterije
        tx_send = 1;               // FLAG za pošiljanje
        if ( CANIsTxReady() )      // Sestavi CAN telegram 2 za MasterA.
        {
            SestaviCAN2();         // Sestavi podatke za CAN telegram
            CANSendMessage(tx_CAN_msg.Address.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
        } 
        V_BatNo = 0;               // Postavi se na prvo baterijo
    }
}*/

//next port - cell for 32 cel master option
void NaslednjiPort()
{
    char j;
	unsigned long LocalcellStatuses=cellStatuses[0];

	V_BatNo++;                      // in gremo na novo baterijo
    rx_cnt = 0;
	
    //setting pins for controllong Xilinx CPLD chip
	//LATD = ((LATD & 0xF0) | (V_BatNo & 0x07));
	LATD=V_BatNo;

	

//tx_CAN_msg.Data[0]=V_BatNo;
//CANSendMessage(tx_CAN_msg.Address.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);


	if (V_BatNo == 32){              // If we read thru all ports
		j=0;

		for(char i=0;i<=28; i+=4){       //four cells per message
			if ( CANIsTxReady() )      // Sestavi CAN telegram 1 za MasterA.
        	{			
		        //memset(Bat_Tx_Str,'\0',sizeof(Bat_Tx_Str)); // Bruši USART buffer
		        //SestaviTxStr();            // Saestavi USART telegram za vse baterije
		        //tx_send = 1;               // FLAG za pošiljanje

    		/*	tx_CAN_msg.Data[0] = (V_Bat[i] >> 8);
    			tx_CAN_msg.Data[1] = (V_Bat[i] & 0xFF);
			    tx_CAN_msg.Data[2] = (V_Bat[i+1] >> 8);
			    tx_CAN_msg.Data[3] = (V_Bat[i+1] & 0xFF);
			    tx_CAN_msg.Data[4] = (V_Bat[i+2] >> 8);
			    tx_CAN_msg.Data[5] = (V_Bat[i+2] & 0xFF);
			    tx_CAN_msg.Data[6] = (V_Bat[i+3] >> 8);
			    tx_CAN_msg.Data[7] = (V_Bat[i+3] & 0xFF);
			    */

				tx_CAN_msg.Data[0] = V_BatL[i];
    			tx_CAN_msg.Data[1] = V_BatH[i];
			    tx_CAN_msg.Data[2] = V_BatL[i+1];
			    tx_CAN_msg.Data[3] = V_BatH[i+1];
			    tx_CAN_msg.Data[4] = V_BatL[i+2];
			    tx_CAN_msg.Data[5] = V_BatH[i+2];
			    tx_CAN_msg.Data[6] = V_BatL[i+3];
			    tx_CAN_msg.Data[7] = V_BatH[i+3];

				// CAN adresses
			    //node_no = (Read_b_eep(0) << 2);
			    //Can_addrLO.SE_ID = 0x040050F3;
			    //Can_addrLO.Bytes[2] = Can_addrLO.Bytes[2] | node_no;
			    //Can_addrHI.SE_ID = 0x040150F3;
			    //Can_addrHI.Bytes[2] = Can_addrHI.Bytes[2] | node_no;
			    Can_addrLO.SE_ID = 0x040050F3;
			    Can_addrLO.Bytes[2] = j;
				Can_addrLO.Bytes[3] = MASTER_NUMBER;
			    tx_CAN_msg.Length.Len = 8;
           		CANSendMessage(Can_addrLO.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
 
		  		j++;
		        V_BatNo = 0;               // Postavi se na prvo baterijo
				LATD=0;
				_delay(1000);
			}
		}      
   		//status message
		//long is dysplaied in reverse order
		//Example: FF FF FF FE - the first battery is not ballancing all others are
		Can_addrLO.Bytes[1] = 1;
		tx_CAN_msg.Data[0] = ((LocalcellStatuses & 0xFF000000L) >> 24);
    	tx_CAN_msg.Data[1] = ((LocalcellStatuses & 0x00FF0000L) >> 16);
		tx_CAN_msg.Data[2] = ((LocalcellStatuses & 0x0000FF00L) >> 8);
	    tx_CAN_msg.Data[3] = (LocalcellStatuses & 0x000000FFL);
		//configuration bits 
		tx_CAN_msg.Data[4] = ((connectedCells & 0xFF000000L) >> 24);
    	tx_CAN_msg.Data[5] = ((connectedCells & 0x00FF0000L) >> 16);
		tx_CAN_msg.Data[6] = ((connectedCells & 0x0000FF00L) >> 8);
	    tx_CAN_msg.Data[7] = (connectedCells & 0x000000FFL);		
		CANSendMessage(Can_addrLO.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
 			   


    }
}

/*********************************************************/
/*                   Main function                       */
/*********************************************************/
void main(void)
{
    //all nonuse ports to inputs to disable reset
//	TRISA = 0xFF; // PortD output
//	TRISB = 0x00; // PortD output
//	TRISC = 0xFF; // PortD output
 //   LATA  = 0x00;
  //  LATB  = 0xFF;
 //   LATC  = 0x00;
    //PORTD = 0xFF;
    //PORTD = 0xFF;
    //PORTD = 0xFF;
	

	TRISD = 0x00; // PortD output
    PORTD = 0xFF;
    LATD  = 0x00;


	//bootloader thru CAN option 
	/*
	Can_bootF.SE_ID = 0x1CC0F3E0; // Boot-loader
    Can_bootF.Bytes[2] = Can_bootF.Bytes[2] | node_no;
    Can_nodeF.SE_ID = 0x0400F4F3; // BMS_Slave -> BMS_master
    Can_nodeF.Bytes[2] = Can_nodeF.Bytes[2] | node_no;
    Can_Mask1.SE_ID = 0x1CFFFFFF;
    Can_Mask2.SE_ID = 0x1CC3FFFF;
    */

	//read battery numbers from EEPROM into array
    ReadBatNo();

    // Initialize TIMER0
    INTCON2bits.TMR0IP = 0;        //Timer0 INT-LOW
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_64);
    WriteTimer0(62411);            // 1/10Sec interval --  8Mhz/4/64 := 3125 -> 65536 - 3125 = 62411
    INTCONbits.GIE = 1;            //enable global interrupts

    // Initialize CAN module with no message filtering
    // CANInitialize(SJW, BRP, PHSEG1, PHSEG2, PROPSEG, config);
    // 8MHz Fosc 250Kb/s
    // 8MHz -> 2MHz @ 250Khz = 8Tq (1+2+3+2)
    CANInitialize(1 ,0x02 ,2 ,3 ,2 , CAN_CONFIG_VALID_XTD_MSG); //256kb at 8Mhz crystal
    // CANInitialize(1 ,0x06 ,2 ,3 ,2 , CAN_CONFIG_VALID_XTD_MSG); USB-CAN
    CANSetOperationMode(CAN_OP_MODE_CONFIG);
    CANSetFilter(CAN_FILTER_B1_F1, Can_bootF.SE_ID, CAN_CONFIG_XTD_MSG);
    CANSetFilter(CAN_FILTER_B1_F2, Can_nodeF.SE_ID, CAN_CONFIG_XTD_MSG);
    CANSetMask(CAN_MASK_B1, Can_Mask1.SE_ID, CAN_CONFIG_XTD_MSG);
    CANSetFilter(CAN_FILTER_B2_F1, Can_addrLO.SE_ID, CAN_CONFIG_XTD_MSG);
    CANSetMask(CAN_MASK_B2, Can_Mask2.SE_ID, CAN_CONFIG_XTD_MSG);
    CANSetOperationMode(CAN_OP_MODE_NORMAL);

	
	//detect reset
	Can_addrLO.Bytes[3] = MASTER_NUMBER;
	Can_addrLO.Bytes[2] = RCON;
   	CANSendMessage(Can_addrLO.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);



	// Initialize USART
	// (8MHz / 16 / 9600) -1 = 9615,3846153846153846153846153846Boud
	// (8MHz / 16 / 1200) -1 = 1199,0407673860911270983213429257Boud
    OpenUSART( USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW, 104);
	// OpenUSART( USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 52);
    tx_cnt = 0;
    tx_send = 0;
    rx_cnt = 0;

    V_BatNo = 0;  // current battery to read from
    // memset(Bat_Tx_Str,'\0',sizeof(Bat_Tx_Str));
	// SestaviTxStr();

    WDTCONbits.SWDTEN = 1;  // Enable Watch Dog

	/*************************************/
	/*      Main application loop        */
    /*************************************/
	while(1)
    {    
  
		/* CAN Bootloader option 
		// Check for CAN message
       	if ( CANIsRxReady() )
        {
            CANReceiveMessage(&rx_CAN_msg.Address.SE_ID, &rx_CAN_msg.Data[0], &rx_CAN_msg.Length.Len, &RecFlags);
            if ( RecFlags & CAN_RX_OVERFLOW )
            {
				// Rx overflow occurred; handle it
            }
            if ( RecFlags & CAN_RX_INVALID_MSG )
            {
				// Invalid message received; handle it
            }
            if ( RecFlags & CAN_RX_XTD_FRAME )
            {
				// Extended Identifier received; handle it
            }
            else
            {
				// Standard Identifier received.
            }
            if ( RecFlags & CAN_RX_RTR_FRAME )
            {
				// RTR frame received
            }
            else
            {
				// Regular frame received.
				// V MasterA nekaj naredi z tem.
            }

			// Extract receiver filter match, if it is to be used
            RxFilterMatch = RecFlags & CAN_RX_FILTER_BITS;
           	if (rx_CAN_msg.Address.SE_ID == Can_bootF.SE_ID)
	        {
               	Write_b_eep (0x00FF,0xFF);
   	        	Busy_eep();
       	        Reset();
                ClrWdt();
            }     
           	if (rx_CAN_msg.Address.SE_ID == Can_nodeF.SE_ID)
           	{
                ClrWdt();
	        }
        }
		*/


		// Process UART Error's
        if (RCSTAbits.OERR == 1)
        {
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
            rx_cnt = 0;
        } 
//V_BatNo=30;
//LATD=V_BatNo;
		// Process UART Battery data
        if ((DataRdyUSART() == 1) || (RCSTAbits.FERR == 1))             // new data in UART
        {
            Rx_Char = ReadUSART();     // Preberi znak iu USART-a
						rx_tic = 0;                // Resetiraj TIME-OUT števec
					
            //if ((Rx_Char == '>') || (Rx_Char == '-'))   // Èakaj na zaèetni znak ali limiter za status
            if ((Rx_Char == '>'))   // Èakaj na zaèetni znak ali limiter za status
            {
                 rx_cnt = 0; //counter of received bytes
                 V_BatChar[rx_cnt++] = '0'; 
            }
            else
            {
 
				if (rx_cnt > 0)        // If we allready received the first sign
                {
    
                    if (Rx_Char == '\0' && V_BatChar[rx_cnt-1]==0x0D && V_BatChar[2]==0x2D) // if we received the whole message
                    {

  						rx_cnt = 0; 
						//V_BatChar[rx_cnt++] = '\0';
                        
						V_BatTemp = hexToInt(&V_BatChar[0]); // Pretvori v vrednost
                        //if (V_BatTemp > 0)                   // Ali je vrednost smiselna
                        //{
                            //V_Bat[V_BatNo] = V_BatTemp;      // Zato jo zapiši
                            //V_Bat[V_BatNo] =0xFF;
							
							//batt statuses
							if(V_BatChar[1]==0x4F) set_bit(cellStatuses[0], V_BatNo); //ballencing 0x4F
							else clear_bit(cellStatuses[0], V_BatNo);
							


							V_BatL[V_BatNo] = V_BatChar[3];	
							V_BatH[V_BatNo] = V_BatChar[4];

/*
tx_CAN_msg.Data[0] =V_BatChar[0];
tx_CAN_msg.Data[1] =V_BatChar[1];
tx_CAN_msg.Data[2] =V_BatChar[2];
tx_CAN_msg.Data[3] =V_BatChar[3];
tx_CAN_msg.Data[4] =V_BatChar[4];
tx_CAN_msg.Data[5] =V_BatChar[5];
tx_CAN_msg.Data[6] =V_BatChar[6];
tx_CAN_msg.Data[7] =V_BatChar[7];

   	CANSendMessage(Can_addrLO.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
*/
							NaslednjiPort();

                        //} 
                        memset(V_BatChar,'\0',sizeof(V_BatChar)); //discard if anything left in UART
                    } else
                    {                                       // Samo spravi sprejeti znak     
                        V_BatChar[rx_cnt++] = Rx_Char;      // in se postavi na naslednji prostor
                    }
                }
            }
        }

		//if there is no data on current port go to next one after timeout
		// we only read from connected batteries
        if ((rx_tic > 2) || (!test_bit(connectedCells,V_BatNo) ) )                // Ali nam je padel TIME-OUT števec
        {
            //V_Bat[V_BatNo] = 0;        // Vpiši niè v rezultat
            V_BatH[V_BatNo] = 255; 
			V_BatL[V_BatNo] = 255; 
			clear_bit(cellStatuses[0], V_BatNo);
			NaslednjiPort();
            rx_tic = 0;
        }

		//debugging thru USART not possible for 32 version since all ports are taken 
        /*if ((tx_cnt > 0)||(tx_send == 1)) // Ali pošiljamo na USART
        {
            if (BusyUSART() == 0)      // Ali USART lahko sprejme znak
            {
                WriteUSART(Bat_Tx_Str[tx_cnt++]); // pošlji podatek
                tx_send = 0;           // Briši FLAg za zaèetek pošiljanja
                if (Bat_Tx_Str[tx_cnt] == '\0') tx_cnt = 0; // Ali smo vse poslali
            }
        }*/


		//read temperature from ds80b20 sensors





        ClrWdt();
    } // Do this forever
}

/*********************************************************/
/*                       interrupts                      */
/*********************************************************/
// High priority interrupt vector
#pragma code InterruptVectorHigh=0x408
void InterruptVectorHigh(void)
{
      _asm
        goto InterruptHandlerHigh  //jump to interrupt routine
      _endasm
}

#pragma code
#pragma interrupt InterruptHandlerHigh

void InterruptHandlerHigh()
{
    unsigned int t1;
    unsigned char mdisp;
    if(PIR1bits.TMR1IF)
    {
        PIR1bits.TMR1IF=0;                    //clear interrupt flag
    }
    if(INTCONbits.TMR0IF)                   //check for TMR0 overflow interrupt flag
    {
        unsigned int t0;
        INTCONbits.TMR0IF = 0;              //clear interrupt flag
        t0=ReadTimer0();
        WriteTimer0(t0 +62411);             //Nastavi na 1/10.0 Sek
        rx_tic++;
    }
}

// Low priority interrupt vector
#pragma code InterruptVectorLow=0x418
void InterruptVectorLow (void)
{
      _asm
          GOTO InterruptHandlerLow  //jump to interrupt routine
      _endasm
}

#pragma code
#pragma interrupt InterruptHandlerLow //save=disp0,disp1,disp2,disp3,TBLPTRU

void InterruptHandlerLow (void)
{
    if(INTCONbits.TMR0IF)                   //check for TMR0 overflow interrupt flag
    {
        unsigned int t0;
        INTCONbits.TMR0IF = 0;              //clear interrupt flag
        t0=ReadTimer0();
        WriteTimer0(t0 +62411);             //Nastavi na 1/10.0 Sek
        rx_tic++;
    }
}
// End of program

