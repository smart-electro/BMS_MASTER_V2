#include <EEP.h>
#define EEPROM_MBS_ID 0x0
#define EEPROM_BAUDRATE 0x1
#define EEPROM_MAX_VOLTAGE 0x2
#define EEPROM_MIN_VOLTAGE 0x8
#define EEPROM_MAX_PWM 0xE

void EepromWrite(unsigned int address, unsigned char data);
unsigned char EepromRead(unsigned int address);