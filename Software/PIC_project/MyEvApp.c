#include "MyEvApp.h"
#include <usart.h>
#include <stdio.h>
/*******************************************************************/
// Function that constructs XML for MyVehicle mobile phone app
/*******************************************************************/
void sendToMyVehicleApp(){
 /* 
  //
  
  
  int i;
  //Serial2.write(27);  
  Serial2.print("  HE"); 
  Serial2.print("<head><donut v='"+String(MyVehicle.SOC)+"'/><ttc v='");
  Serial2.print(MyVehicle.ttc,2);
  Serial2.print("'/><rd v='"+String(MyVehicle.rd)+"'/><amp v='"+String(MyVehicle.amp,1)+"'/>");
  for( i = 9; i>=0; i--){    // print chart data
    if(MyVehicle.pw[i]>150) MyVehicle.pw[i]=150;
    Serial2.print("<pw  v='"+String(MyVehicle.pw[i],0)+"'/>");
  }
  
  //Serial.print("<pw  v='"+MyVehicle.pw1+"'/><pw  v='"+MyVehicle.pw2+"'/>");
  //Serial.print("<pw  v='"+String(MyVehicle.pw3)+"'/><pw  v='"+String(MyVehicle.pw4)+"'/><pw  v='"+String(MyVehicle.pw5)+"'/><pw  v='"+String(MyVehicle.pw6)+"'/><pw  v='"+String(MyVehicle.pw7)+"'/><pw  v='"+String(MyVehicle.pw8)+"'/>");
  //Serial.print("<pw  v='"+String(MyVehicle.pw9)+"'/><pw  v='"+String(MyVehicle.pw10)+"'/><pw  v='"+String(MyVehicle.pw11)+"'/>
  Serial2.print("<vol v='"+String(MyVehicle.vol,2)+"'/><camp v='"+String(MyVehicle.camp)+"'/><temp v='"+String(MyVehicle.temp,1)+"'/>");
  Serial2.print("<max v='"+String(MyVehicle.maxV,2)+"'/><min v='"+String(MyVehicle.minV,2)+"'/><avg v='"+String(MyVehicle.avgV,2)+"'/><mode v='"+String(MyVehicle.mode)+"'/><mode2 v='"+String(MyVehicle.mode)+"'/>");


  for( i = 0; i < ((NO_OF_BMS_MASTERS)*6); i++)    // print the CELL data
  {
 	Serial2.print("<bt id='"+String(i+1)+"' v='"+String(BmsVoltage[i],2)+"'");// c='4'/>"); //1-orange, 2-blue , 3 green,o-red
   // Serial2.print("<bt id='"+String(i+1)+"' v='"+String(BmsVoltage[i],2)+"' c='1'/>");
    if(MyVehicle.CellBalance[i] >0 )Serial2.print(" c='3'/>");
    else if(BmsVoltage[i]< BATTERY_CUT_OFF_VOLTAGE) Serial2.print(" c='0'/>"); //1-orange, 2-blue , 3 green,o-red
    else Serial2.print(" c='2'/>"); //normal

     
  } 
    Serial2.print("<dht11 v='85' /><dht11 v='25' /></head>");
  Serial2.print("/HE");
  */
float voltage;
char str[80];

   


	// Initialize USART
	// (8MHz / 16 / 9600) -1 = 9615,3846153846153846153846153846Boud
	// (8MHz / 16 / 1200) -1 = 1199,0407673860911270983213429257Boud
	// USART_BRGH_LOW /46
    OpenUSART( USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 12);
_delay(1000);
 putsUSART("HE"); 
 putsUSART("<head><donut v='86");

 

 putsUSART("'/> <ttc v='0'/> <rd v='0'/> <amp v='0'/>");
 putsUSART("<pw  v='0'/><pw  v='0'/><pw  v='0'/><pw  v='0'/><pw  v='0'/><pw  v='0'/><pw  v='0'/><pw  v='0'/>");
 putsUSART("<pw  v='0'/><pw  v='0'/><pw  v='0'/>");
 putsUSART("<vol v='0'/><camp v='0'/> <temp v='0'/>");
 putsUSART("<max v='4.22'/><min v='2.22'/><avg v='3.22'/><mode v='3'/><mode2 v='3'/>");
 //putsUSART("<bt id='1' v='3.0' c='3'/> <bt id='2' v='1.1' c='0'/><bt id='3' v='1.8' c='1'/><bt id='4' v='2.6' c='2'/> <bt id='5' v='1.0' c='0'/> <bt id='6' v='1.6' c='1'/><bt id='7' v='2.2' c='2'/><bt id='8' v='1.9' c='1'/><bt id='9' v='3.3' c='3'/><bt id='10' v='2.0' c='2'/><bt id='11' v='3.1' c='3'/><bt id='12' v='1.2' c='0'/><bt id='13' v='3.3' c='3'/><bt id='14' v='3.3' c='3'/><bt id='15' v='3.3' c='3'/><bt id='16' v='3.3' c='3'/>");
 
for (char i=0;i<32;i++){
//(!test_bit(connectedCells,V_BatNo) ) )  
	voltage=(int)(V_BatH[i]+(int)(V_BatL[i]*256));
	voltage/=1000;
	if(voltage<=0)voltage=0;

	putsUSART("<bt id='");
	sprintf(str, "%d", (i+1));
	putsUSART(str);
	putsUSART("' v=");
	
	//sprintf(str, "%X, %X, %.3f", V_BatH[i],V_BatL[i],voltage);
	sprintf(str, "'%.2f'",voltage);
	putsUSART(str);


	if(test_bit(cellStatuses[0],i)) putsUSART(" c='3'/>");
    else putsUSART(" c='2'/>"); //1-orange, 2-blue , 3 green,o-red
   
}

putsUSART("<dht11 v='85' /><dht11 v='25' /></head>");
 putsUSART("/HE");

    OpenUSART( USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW, 104);
_delay(1000);

//	putsUSART("Hello world/r/n");


}

/*******************************************************************/
// Function that fills up array of 12 bytes with floating avarage
/*******************************************************************/
void floatingAvarage(char data){
/*  float delta;

  if(FilterIndex >= 10){ 
      FilterIndex=0;
  }
  
  delta=0.7;
  MyVehicle.pw[0]=MyVehicle.pw[1]*(1-delta)+ data*delta;

Serial.print(data);
  for(byte i=1;i<10;i++){
      delta=1/((float)i*2);
      Serial.print("dt=");
      Serial.print(String(delta,5));
      MyVehicle.pw[i]=MyVehicle.pw[i]*(1-delta)+ MyVehicle.pw[i-1]*delta;
      Serial.print(" ");
      Serial.print(String(i));
      Serial.print("=");
      Serial.print(MyVehicle.pw[i]);
  }
  Serial.println();
  */


}