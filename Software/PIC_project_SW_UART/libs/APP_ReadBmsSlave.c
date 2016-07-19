#include "BSW_UART.h"
#include "APP_ReadBmsSlave.h"
#include "APP_AuxFunctions.h"
#include "BSW_GPIO.h"
#include <p18cxxx.h>
#include "BSW_CAN.h"
#include "BSW_EEPROM.h"
#include "RTOS.h"
#include <usart.h>
#include <stdio.h>
unsigned char rx_cnt=0;
unsigned char crc8=0;
unsigned char BatChar[8];
				unsigned int ADC_value, ADC_Ref;  
				float V_BatTemp;  

void processReceivedByte(char Rx_Char);
void NaslednjiPort();
unsigned char crc_ibutton_update(unsigned char crc8,unsigned char datain);

#define CELL_BALANCING 0x4f  // 0 character
#define CELL_CUTOFF_VOLTAGE 50  // 250-200
#define CELL_EMPTY_VOLTAGE 100  // 300-200
#define CELL_FULL_VOLTAGE 220  // 420-200
#define CELL_FULLOFFSET_VOLTAGE 210  // 410-200
#define CELL_OVERVOLTAGE_VOLTAGE 230  // 430-200

#define STATUS_B1_ALL_FULL 0
#define STATUS_B1_ONE_FULL 1
#define STATUS_B1_EMPTY 2
#define STATUS_B1_CUTOFF 3
#define STATUS_B1_OVERVOLTAGE 4
#define STATUS_B0_ALL_ROK 6

//cell status //0-normal , 1-full, 2-empty, 3-cut-off 4-overvoltage 5-NoRead
#define CELL_NORMAL 0
#define CELL_FULL 1
#define CELL_EMPTY 2
#define CELL_CUTOFF 3
#define CELL_OVERVOLTAGE 4
#define CELL_NOROK 5





void readBmsSlaves()
{


		//sw uart read data
		// Process UART Error's
        if (RCSTAbits.OERR == 1)
        {
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
            rx_cnt = 0;
        } 
		// Process UART Battery data
        if ((DataRdyUSART() == 1) || (RCSTAbits.FERR == 1)){             // new data in UART
			//putsUSART(".");
			processReceivedByte(ReadUSART());
		}

		//receive timeout
		if(SlaveReceiveTimeout >= 2) {
//putsUSART("x");
			SlaveReceiveTimeout=0;
			//slaves can have +-40% baudrate. If timeout-> start sweping the baudrate (timer0)
			// 8Mhz/4 = 2M samples/s
	
			//			    1110*3 -> WriteTimer0(65535-600);64935
			//			    1130*3 -> WriteTimer0(65535-589);64946
			//				1150*3 -> WriteTimer0(65535-580);64955
			//			    1170*3 -> WriteTimer0(65535-569);64966
			//			    1190*3 -> WriteTimer0(65535-560);64975
			//default		1200*3 -> WriteTimer0(65535-555);64980
			//				1240*3 -> WriteTimer0(65535-537);64998 //detected on one Tiny
			//			    1250*3 -> WriteTimer0(65535-533);65002

			SlaveReceiveMasterTimeout++;
			Timer0Value+=5;
 		//	if(Timer0Value >= 65535) Timer0Value=64935;//65100
 		//	if(Timer0Value <= 61935) Timer0Value=64935;
			if(Timer0Value >= 65346) Timer0Value=65135;
 			if(Timer0Value <= 65135) Timer0Value=65135;

			//if no readout after sweep -> we go for next cell
			if(SlaveReceiveMasterTimeout >= 70) {	
				BatteryCells[BatNo].status=CELL_NOROK;
				BatteryCells[BatNo].Voltage=0xff; //clear values
				BatteryCells[BatNo].PWM=0xff;
				clear_bit(BmsStatus0, BatNo);     //clear status bit
				NaslednjiPort();
			}
		}



}
void NaslednjiPort()
{
//putsUSART("naslednji port:");
	//unsigned long LocalcellStatuses=cellStatuses[0];
	SlaveReceiveTimeout=0;
	SlaveReceiveMasterTimeout=0;
	Timer0Value=65285; //default value for 1200kb baud.

    BatNo++;                      // in gremo na novo baterijo
	LATD=BatNo;				  //setting pins for controllong Xilinx CPLD chip


//putsUSART((BatNo);


	if (BatNo >= 16){              // If we read thru all ports TODO only one marked!
		BatNo = 0;
		
		//check if all cells are full
		unsigned char countFull=0;
		unsigned char countROK=0;
		set_bit(BmsStatus0, 7); //we set unused but so if everithing is ok status byte0 is 0xFF
		BmsStatus1=0;  
		//loop through all cells
		for(char i=0; i<=5; i++){ 
			if(BatteryCells[i].status == CELL_FULL){
				countFull++;
				set_bit(BmsStatus1, STATUS_B1_ONE_FULL); //one full flag 
 			}
			if(BatteryCells[i].status != CELL_NOROK){
				 countROK++;
  			}
			if(BatteryCells[i].status==CELL_OVERVOLTAGE){
		 		set_bit(BmsStatus1, STATUS_B1_OVERVOLTAGE ); //OVERVOLTAGE  
 			}
			if(BatteryCells[i].status==CELL_CUTOFF){
		 		set_bit(BmsStatus1, STATUS_B1_CUTOFF ); //OVERVOLTAGE  
 			}
			if(BatteryCells[i].status==CELL_EMPTY){
		 		set_bit(BmsStatus1, STATUS_B1_EMPTY ); //OVERVOLTAGE  
 			}
		}
		if(countFull == 6 ) set_bit(BmsStatus1, STATUS_B1_ALL_FULL); //all full flag
		if(countROK  == 6 ) set_bit(BmsStatus0, STATUS_B0_ALL_ROK); 
		else clear_bit(BmsStatus0, STATUS_B0_ALL_ROK);
	}	
    rx_cnt = 0;

}

unsigned char crc_ibutton_update(unsigned char crc8,unsigned char datain)
{
  unsigned char i;

  crc8 = crc8 ^ datain;
  for (i = 0; i < 8; i++)
    {
      if (crc8 & 0x01)
    	crc8 = (crc8 >> 1) ^ 0x8C;
      else
    	crc8 >>= 1;
    }
  return crc8;
}

void processReceivedByte(char Rx_Char){
	char str[80];
	sprintf(str, "'%x'",Rx_Char);
	//putsUSART(str);	
	
	if ((Rx_Char == '>'))   // Èakaj na zaèetni znak ali limiter za status
	{
		rx_cnt = 0; //counter of received bytes
//		BatChar[rx_cnt++] = '0'; 
		BatChar[rx_cnt++] = Rx_Char; 
	}
	else
	{
		if (rx_cnt > 0)        // If we allready received the first sign
		{    
			if (rx_cnt==7 && BatChar[7]==0xEE) // if we received the whole message
			{
				rx_cnt = 0; 
				//putsUSART("DOBIL1!");
				//crc carculation
				crc8=0;
				//BatChar[0]= '>';
				for (unsigned char i=0; i<=5; i++)
			  	{
			    	crc8 = crc_ibutton_update(crc8, BatChar[i]);
			  	}
				if(crc8==BatChar[6]){
					//putsUSART("DOBIL!");
					/*	
					softuart_putchar('>');
					softuart_putchar((ADC_result & 0xFF00L) >> 8);   // Apply the lower middle byte of the data 
					softuart_putchar((ADC_result & 0x00FFL));      // Apply the low byte of the data 
					softuart_putchar((RefVolt & 0xFF00L) >> 8);   // Apply the lower middle byte of the data
					softuart_putchar((RefVolt & 0x00FFL));      // Apply the low byte of the data
					softuart_putchar(PWM);
					softuart_putchar(crc8);*/
	
					ADC_value=(((unsigned int)(BatChar[1]))<<8) + BatChar[2];
					ADC_Ref=(((unsigned int)(BatChar[3]))<<8) + BatChar[4];
					V_BatTemp = (float)ADC_value * (float)420;
					V_BatTemp /= ADC_Ref;	
					V_BatTemp -= (float)200;
					BatteryCells[BatNo].Voltage=(unsigned char)V_BatTemp;
	                
					//Cell voltage calcualtion        
					/*unsigned int V_BatTemp;  
					V_BatTemp=(((unsigned int)(BatChar[3]))<<8) + BatChar[4];			
					V_BatTemp=V_BatTemp-2000;
					V_BatTemp=((unsigned int)V_BatTemp)/10;
					BatteryCells[BatNo].Voltage=(char)V_BatTemp;	*/				
					
					//pwm
					BatteryCells[BatNo].PWM=BatChar[5];
					
					//read OK
					set_bit(BmsStatus0, BatNo);
						
					//status //0-normal , 1-full, 2-empty, 3-cut-off 4-overvoltage 5-NoRead
					//if status was set to FULL and voltage has droped below OFFSET the cell is not full anymore
					if(BatteryCells[BatNo].Voltage<= CELL_FULLOFFSET_VOLTAGE){
					 	BatteryCells[BatNo].status=CELL_NORMAL;
					}
					//if balancing the cell is full
					if(BatteryCells[BatNo].PWM > 0){ 
						BatteryCells[BatNo].status=CELL_FULL; 
					}
					//if cell is empty
					if(BatteryCells[BatNo].Voltage<= CELL_EMPTY_VOLTAGE){
					 	BatteryCells[BatNo].status=CELL_EMPTY;
					}
					//if cell is at absolute minimum
					if(BatteryCells[BatNo].Voltage<= CELL_CUTOFF_VOLTAGE){
					 	BatteryCells[BatNo].status=CELL_CUTOFF;
					}
					//if cell is at absolute maximum
					if(BatteryCells[BatNo].Voltage>= CELL_OVERVOLTAGE_VOLTAGE){
					 	BatteryCells[BatNo].status=CELL_OVERVOLTAGE;
					}



					//Cell max voltage history
					if((BatteryCells[BatNo].Voltage > BatteryCells[BatNo].MaxVoltage) || (BatteryCells[BatNo].MaxVoltage==0xFF)){
						BatteryCells[BatNo].MaxVoltage=BatteryCells[BatNo].Voltage;
						EepromWrite(EEPROM_MAX_VOLTAGE+BatNo, BatteryCells[BatNo].MaxVoltage);
					}
					//Cell min voltage checking
					if((BatteryCells[BatNo].Voltage < BatteryCells[BatNo].MinVoltage) || (BatteryCells[BatNo].MinVoltage==0xFF)){
						BatteryCells[BatNo].MinVoltage=BatteryCells[BatNo].Voltage;
						EepromWrite(EEPROM_MIN_VOLTAGE+BatNo, BatteryCells[BatNo].MinVoltage);
					}				
					//Cell max Balancing checking
					if((BatteryCells[BatNo].PWM > BatteryCells[BatNo].MaxBalancing) || (BatteryCells[BatNo].MaxBalancing==0xFF)){
						BatteryCells[BatNo].MaxBalancing=BatteryCells[BatNo].PWM;
						EepromWrite(EEPROM_MAX_PWM+BatNo, BatteryCells[BatNo].MaxBalancing);
					}
	
		
				
				
					NaslednjiPort();
				}
			} else
           	{                                       // Samo spravi sprejeti znak     
       			BatChar[rx_cnt++] = Rx_Char;      // in se postavi na naslednji prostor
            }
		}
	}
}
