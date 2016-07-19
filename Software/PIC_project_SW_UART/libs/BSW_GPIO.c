#include <p18cxxx.h>
#include "BSW_GPIO.h"

void GpioConfig(){
	TRISBbits.TRISB0= 0x0;  //led1
	TRISBbits.TRISB1= 0x0;	//led2


	//ADCON0 = 0x00; //disable A/D converter 
	//ADCON1 = 0x0F; //select all digital 
	TRISD = 0x00; // PortD output
    PORTD = 0xFF;
    LATD  = 0x00;

}


