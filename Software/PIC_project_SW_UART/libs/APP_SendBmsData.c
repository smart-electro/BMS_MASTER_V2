#include <p18cxxx.h>
#include "APP_ReadBmsSlave.h"
#include "APP_AuxFunctions.h"
#include "APP_ReadTemperature.h"
#include "BSW_CAN.h"
#include "APP_Settings.h"
#include "APP_SendBmsData.h"

unsigned char CAN_Data[8];
unsigned long CAN_Identifier;

/*******************************************/
/*******************************************/
void sendBmsStatus()
{
	if(sendBmsStatus_send==1 && CANIsTxReady()){ 
		sendBmsStatus_send=0;

		#define CAN_lenghth1 7
		CAN_Data[0]=BmsStatus0;
		CAN_Data[1]=BmsStatus1;
		CAN_Data[2]=0;//(unsigned char)Temperature[0];
		CAN_Data[3]=0;//(unsigned char)Temperature[1];
		CAN_Data[4]=0;//(unsigned char)Temperature[2];
		CAN_Data[5]=0;//(unsigned char)Temperature[3];
		CAN_Data[6]=bms_id;
		
		// CAN adresses
		//todo read from eeprom
		CAN_Identifier = (((unsigned long)0x18) << 24) | (((unsigned long)0xff) << 16) | (((unsigned long)bms_id) << 8) | (unsigned long)0x10;
		
		//send message
		CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth1, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
	}
}

/*******************************************/
/*******************************************/
void sendCellVoltages()
{
	if(sendCellVoltages_send==1 && CANIsTxReady()){ 
		sendCellVoltages_send=0;
		// CAN data
		#define CAN_lenghth2 6
		CAN_Data[0]=BatteryCells[0].Voltage;
		CAN_Data[1]=BatteryCells[1].Voltage;
		CAN_Data[2]=BatteryCells[2].Voltage;
		CAN_Data[3]=BatteryCells[3].Voltage;
		CAN_Data[4]=BatteryCells[4].Voltage;
		CAN_Data[5]=BatteryCells[5].Voltage;	
	
		
		
		// CAN adresses
		//todo read from eeprom
		CAN_Identifier = (((unsigned long)0x18) << 24) | (((unsigned long)0xff) << 16) | (((unsigned long)bms_id) << 8) | (unsigned long)0x11;
		
		//send message
		CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth2, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);

	}
}

/*******************************************/
/*******************************************/
void sendCellBalancingStatus()
{
	if(sendCellBalancingStatus_send==1 && CANIsTxReady()){ 	
		sendCellBalancingStatus_send=0;

		#define CAN_lenghth3 6
		CAN_Data[0]=BatteryCells[0].PWM;
		CAN_Data[1]=BatteryCells[1].PWM;
		CAN_Data[2]=BatteryCells[2].PWM;
		CAN_Data[3]=BatteryCells[3].PWM;
		CAN_Data[4]=BatteryCells[4].PWM;
		CAN_Data[5]=BatteryCells[5].PWM;
		
		// CAN adresses
		//todo read from eeprom
		CAN_Identifier = (((unsigned long)0x18) << 24) | (((unsigned long)0xff) << 16) | (((unsigned long)bms_id) << 8) | (unsigned long)0x12;
		
		//send message
		CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth3, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
	}
}





/*******************************************/
/*******************************************/
void sendCellMaxVoltage()
{
	if(sendCellMaxVoltage_send==1 && CANIsTxReady()){ 
		sendCellMaxVoltage_send=0;

		// CAN data
		#define CAN_lenghth5 6
		CAN_Data[0]=BatteryCells[0].MaxVoltage;
		CAN_Data[1]=BatteryCells[1].MaxVoltage;
		CAN_Data[2]=BatteryCells[2].MaxVoltage;
		CAN_Data[3]=BatteryCells[3].MaxVoltage;
		CAN_Data[4]=BatteryCells[4].MaxVoltage;
		CAN_Data[5]=BatteryCells[5].MaxVoltage;
		
		// CAN adresses
		//todo read from eeprom
		CAN_Identifier = (((unsigned long)0x18) << 24) | (((unsigned long)0xff) << 16) | (((unsigned long)bms_id) << 8) | (unsigned long)0x13;

		
		//send message
		CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth5, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
	}
}	


/*******************************************/
/*******************************************/
void sendCellMinVoltage()
{
	if(sendCellMinVoltage_send==1 && CANIsTxReady()){ 
		sendCellMinVoltage_send=0;

		// CAN data
		#define CAN_lenghth6 6
		CAN_Data[0]=BatteryCells[0].MinVoltage;
		CAN_Data[1]=BatteryCells[1].MinVoltage;
		CAN_Data[2]=BatteryCells[2].MinVoltage;
		CAN_Data[3]=BatteryCells[3].MinVoltage;
		CAN_Data[4]=BatteryCells[4].MinVoltage;
		CAN_Data[5]=BatteryCells[5].MinVoltage;
		
		
		// CAN adresses
		//todo read from eeprom
		CAN_Identifier = (((unsigned long)0x18) << 24) | (((unsigned long)0xff) << 16) | (((unsigned long)bms_id) << 8) | (unsigned long)0x14;

		
		//send message
		CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth6, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
	}
}

/*******************************************/
/*******************************************/
void sendCellMaxBalancing()
{
	if(sendCellMaxBalancing_send==1 && CANIsTxReady()){ 
		sendCellMaxBalancing_send=0;

		// CAN data
		#define CAN_lenghth4 6
		CAN_Data[0]=BatteryCells[0].MaxBalancing;
		CAN_Data[1]=BatteryCells[1].MaxBalancing;
		CAN_Data[2]=BatteryCells[2].MaxBalancing;
		CAN_Data[3]=BatteryCells[3].MaxBalancing;
		CAN_Data[4]=BatteryCells[4].MaxBalancing;
		CAN_Data[5]=BatteryCells[5].MaxBalancing;
		
		// CAN adresses
		//todo read from eeprom
		CAN_Identifier = (((unsigned long)0x18) << 24) | (((unsigned long)0xff) << 16) | (((unsigned long)bms_id) << 8) | (unsigned long)0x15;
		
		//send message
		CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth4, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
	}
}