/*----------------------------------------------------------------------------*/
/*  T15a.c			- Interrupt high/low priority ISR - demo example		  */
/*							  Timer0 overflow interrupt (high priority)		  */
/*							  Timer1 overflow interrupt (low priority) 		  */
/*																																				    */
/*  Project			- T15a (MPLAB C18)										  */
/*  Tools				- MPLAB v7.50, MCC18 "C" Compiler					  */						
/*	Hardware		- PICDEM 2 Plus, pic18F452 device						  */
/*																																				    */
/*	Company			- CVUT FEL K13114										  */
/*  Author			- (c) J.Zdenek, 2008,2009     	      			  		  */
/*	Notices			- tab=2, ## search mark              					  */
/*  Revision		- V1.0						      						  */
/*  Status			- debugged	  			      	      					  */
/* 									      									  */
/*----------------------------------------------------------------------------*/

#include <p18cxxx.h>
#include <timers.h>
#include "BSW_Timers.h"
//#include "BSW_GPIO.h"
/*#include "BSW_SoftUART.h"
#include "BSW_ReadADC.h"*/



void RTOS_TimerConfig(){

	// === Timer 0 === Hi priority
	// L1-page 103,104 Fig.10-1, Timer 0 Control Register 
//0b10011111;
	T0CON &= 0b10010000;			// TMR0ON=1 -> Timer enabled
									// T08BIT=0 -> 16 bit timer/counter
									// T0CS  =0 -> timer internal clock Fosc/4
									// T0SE  =X -> unimportant
									// PSA   =1 -> prescaler disabled (bypassed)
									// T0PS2:T0PS0 -> unimportant

	// === Timer 1 ===
	// L1-page 107,108 Fig.11-2, Timer 1 Control Register
	//
	T1CONbits.RD16 = 1;				// Enable TMRH/L R/W atomic oparation
	T1CKPS1=1;
	T1CKPS0=1;						// T1CKPS1:T1CKPS0=0,0 -> prescaler 8
									// T1OSCEN -> Timer 1 spec.osc.disabled
							    	// T1SYNCnon -> unimportant if internal clock
									// TMR1CS=0 -> enable internal clock Fosc/4 
	T1CONbits.TMR1ON = 1; 			// Enable Timer1
	TMR1H = 0;						// Clear TMR1H buffer reg 
	TMR1L = 0;						// Clear TMRH/L counting registers, order important

	// === Interrupt system initialization ===
	// L1-page 75,74 Fig.8-1, INTCON Register, Interrupt logic
	// Interrupt sources control bits:
	// xxxxIP=0/1 -> low/high priority level  (Interrupt Priority Bit)
	// xxxxIE=0/1 -> interrupt disable/enable (Interrupt Enable Bit)
	// xxxxIF=0/1 -> interrupt pending bit   	(Interrupt Flag Bit)
	//				=== !!! xxxxIF bit is set by HW but have to be reset by SW !!! ===
	//
	INTCON2bits.TMR0IP = 1;		    // Timer 0 -> high priority interrupt group
	INTCONbits.TMR0IE=1;			// Enable Timer0 interrupt

	IPR1bits.TMR1IP = 0;			// Timer 1 -> Low priority interrupt group
	PIE1bits.TMR1IE = 1; 			// Enable Timer1 interrupt

	RCONbits.IPEN = 1;				// Enable interrupt system priority feature
	INTCONbits.GIEH = 1;			// Enable high priority interrupts
	INTCONbits.GIEL = 1; 			// Enable low priority interrupts

	

}
