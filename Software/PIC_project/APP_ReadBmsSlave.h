/*struct Cells_statuses{
	unsigned  CELL1_ROK : 1;
	unsigned  CELL2_ROK : 1;
	unsigned  CELL3_ROK : 1;
	unsigned  CELL4_ROK : 1;
	unsigned  CELL5_ROK : 1;
	unsigned  CELL6_ROK : 1;
	unsigned  ALL_ROK : 1;
	unsigned  : 1;
};
struct Cells_statuses Cells_status;*/
unsigned char SlaveReceiveTimeout=0;
unsigned char SlaveReceiveMasterTimeout=0;
void readBmsSlaves();
unsigned char BatNo=0;

unsigned char BmsStatus0;
unsigned char BmsStatus1;

#define NO_CELLS 6
struct BatteryCell{ 
	unsigned char Voltage; 
	unsigned char PWM; 
	unsigned char MaxVoltage;
	unsigned char MinVoltage;
	unsigned char MaxBalancing;
	unsigned char status;  //0-normal , 1-full, 2-low, 3-empty, 4-cuttoff, 5-NoRead
};
struct BatteryCell BatteryCells[NO_CELLS];
