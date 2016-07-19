#include <p18cxxx.h>
#include "APP_Settings.h"
#include "APP_ReadBmsSlave.h"
#include "BSW_CAN.h"
#include "BSW_EEPROM.h"



enum CAN_RX_MSG_FLAGS CAN_RecFlags;
unsigned char CAN_Data[8];
	unsigned char buffer[4];
unsigned char CAN_Length;

void checkForCan(){
	unsigned long CAN_Identifier;

	// Check for CAN message
	if(CANIsRxReady()){
	
		CANReceiveMessage(&CAN_Identifier, &CAN_Data[0], &CAN_Length, &CAN_RecFlags);
	    if ( CAN_RecFlags & CAN_RX_OVERFLOW )
	    {
			// Rx overflow occurred; handle it
	    }
	    if ( CAN_RecFlags & CAN_RX_INVALID_MSG )
	    {
			// Invalid message received; handle it
	    }
	    if ( CAN_RecFlags & CAN_RX_XTD_FRAME )
	    {
			// Extended Identifier received; handle it
	    }
	    else
	    {
			// Standard Identifier received.
	    }
	    if ( CAN_RecFlags & CAN_RX_RTR_FRAME )
	    {
			// RTR frame received
	    }
	    else
	    {
			// Regular frame received.
			// V MasterA nekaj naredi z tem.
	    }
	

			// Extract receiver filter match, if it is to be used
		
			buffer[0] = (CAN_Identifier >> 24) & 0xFF;
			buffer[1] = (CAN_Identifier >> 16) & 0xFF;
			buffer[2] = (CAN_Identifier >> 8) & 0xFF;
			buffer[3] = CAN_Identifier & 0xFF;
	 		if ((buffer[0] == 0x18) && (buffer[2] == bms_id)&& (buffer[3]==0x00)) 
	 		{
					EepromWrite(EEPROM_MBS_ID, CAN_Data[3]);
	    			EepromWrite(EEPROM_BAUDRATE, CAN_Data[2]);
	     	    	Reset();
			}
	}
}


void readSettings(){
	//bmsID
	bms_id=EepromRead(EEPROM_MBS_ID);

	//can baud rate
	BaudrateSettings=EepromRead(EEPROM_BAUDRATE);

	//celll history
	for(int i=0;i<=5;i++){
		BatteryCells[i].MaxVoltage=EepromRead(EEPROM_MAX_VOLTAGE+i);
		BatteryCells[i].MinVoltage=EepromRead(EEPROM_MIN_VOLTAGE+i);
		BatteryCells[i].MaxBalancing=EepromRead(EEPROM_MAX_PWM+i);
	}


}