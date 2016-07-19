#define TABLE_LEN 200
#define ADC_ChanalNo 4

signed int TdC(unsigned int tp_temp);    //Temperatura iz LOOK-up tabele
int Temperature[ADC_ChanalNo];
unsigned char Temperatura(unsigned int tp_temp);    //Temperatura iz LOOK-up tabele

void CalcTemps();
