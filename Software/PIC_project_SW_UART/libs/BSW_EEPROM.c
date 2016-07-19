#include "BSW_EEPROM.h"


void EepromWrite(unsigned int address, unsigned char data){
	Write_b_eep (address, data);

	Busy_eep();
}

unsigned char EepromRead(unsigned int address){
	return	Read_b_eep( address );
}


