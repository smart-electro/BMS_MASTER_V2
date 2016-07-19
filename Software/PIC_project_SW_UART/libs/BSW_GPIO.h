void GpioConfig();
#define LED1_ON		PORTBbits.RB0=1;
#define LED1_OFF	PORTBbits.RB0=0;
#define LED1_TOGGLE	LATBbits.LATB0 = ~LATBbits.LATB0;



#define LED2_ON		PORTBbits.RB1=1;
#define LED2_OFF	PORTBbits.RB1=0;
#define LED2_TOGGLE	LATBbits.LATB1 = ~LATBbits.LATB1;
#define LED2_STATUS	PORTBbits.RB1
