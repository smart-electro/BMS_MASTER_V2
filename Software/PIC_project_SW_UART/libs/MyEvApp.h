//MyVehicle mobile phone app struct definition
struct AvailableData {
  char SOC;     //battery capacity in circle on "Main"
  float ttc;     //time to charge "Main"
  char rd;      //remaining distance "Main"
  float amp;     //current on "MAIN"
  float vol;     //voltega on "Battery" 
  char camp;    //temperature on "Battery"
  float temp;    //current on "battery"    
  float maxV;    //chart point "Main"
  float minV;    //chart point "Main"
  float avgV;    //chart point "Main"
  char mode;    //header 3-charging, 2-driving, 1-off,

  float pw[10];     //chart point "Main"
 
  unsigned int CellVoltage[30]; //voltages of invididual cells from BMS
  unsigned char CellBalance[30]; //voltages of invididual cells from BMS

}MyVehicle;
#define set_bit(ADDRESS,BIT) (ADDRESS |= (1L<<BIT))
#define clear_bit(ADDRESS,BIT) (ADDRESS &= ~(1L<<BIT))
#define toggle_bit(ADDRESS,BIT) (ADDRESS ^= (1L<<BIT))
#define test_bit(ADDRESS,BIT) (ADDRESS & (1L<<BIT))

char V_BatL[32];                                 // Rezultati meritev vseh 8 beterij
char V_BatH[32];                                 // Rezultati meritev vseh 8 beterij
char V_BatNo;                                    // Stevilka baterije iz katere beremo podatke
unsigned long cellStatuses[10];                  // bug in compiler!!!! we dont need array!
void sendToMyVehicleApp();