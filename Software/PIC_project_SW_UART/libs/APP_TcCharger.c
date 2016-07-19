#include <p18cxxx.h>
#include "APP_ReadBmsSlave.h"
#include "APP_AuxFunctions.h"
#include "APP_ReadTemperature.h"
#include "BSW_CAN.h"
#include "APP_Settings.h"
#include "APP_SendBmsData.h"
#include "APP_TcCharger.h"
unsigned char CAN_Data[8];
unsigned long CAN_Identifier;

/*******************************************/
/*******************************************/
void sendTcChargerCommand()
{
	if(sendTcChargerCommand_send==1 && CANIsTxReady()){ 
		sendTcChargerCommand_send=0;

		#define CAN_lenghth1 8
		CAN_Data[0]=0x2;
		CAN_Data[1]=0x8A;
		CAN_Data[2]=0x0;
		CAN_Data[3]=0xC8;

		CAN_Data[4]=0;;
		CAN_Data[5]=0;;
		CAN_Data[6]=0;
		CAN_Data[7]=0;
		
		// CAN adresses
		//todo read from eeprom

		CAN_Identifier =0x1806E5F4;

//        can_tx[0]=0x06; 
//        can_tx[1]=0xA6;/ 1702
//        can_tx[2]=0x00;
//        can_tx[3]=0x6E; 110
//        can_tx[4]=0x00;
//        can_tx[5]=0x00;
//        can_tx[6]=0x00;
//        can_tx[7]=0x00;


		//CAN_Identifier = (((unsigned long)0x18) << 24) | (((unsigned long)0xff) << 16) | (((unsigned long)bms_id) << 8) | (unsigned long)0x10;
		
		//send message
		CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth1, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
	}
}

