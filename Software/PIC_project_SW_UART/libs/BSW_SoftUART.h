/*
#if !defined(F_CPU)
#warning "F_CPU not defined in makefile - now defined in softuart.h"
#define F_CPU 3686400UL
#endif
*/

//#define SOFTUART_BAUD_RATE      1200

//#if defined (__AVR_ATtiny25__) || defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__)
/*#include "sw_uart1.h"
#define SWTXD         PORTAbits           // Transmit pin port and pin
#define SWTXDpin      RA0
#define TRIS_SWTXD    TRISAbits           // Transmit pin tris and pin
#define TRISbit_SWTXD TRISA0

#define SWRXD         PORTAbits           // Receive pin port and pin
#define SWRXDpin      RA1
#define TRIS_SWRXD    TRISAbits           // Receive pin tris and pin
#define TRISbit_SWRXD TRISA1
*/

/*
#define SOFTUART_RXPIN   PORTAbits
#define SOFTUART_RXDDR   TRISAbits
#define SOFTUART_RXBIT   RA1

#define SOFTUART_TXPORT  PORTAbits
#define SOFTUART_TXDDR   TRISAbits
#define SOFTUART_TXBIT   RA1
*/



/*
#define SOFTUART_T_COMP_LABEL      TIM0_COMPA_vect
#define SOFTUART_T_COMP_REG        OCR0A
#define SOFTUART_T_CONTR_REGA      TCCR0A
#define SOFTUART_T_CONTR_REGB      TCCR0B
#define SOFTUART_T_CNT_REG         TCNT0
#define SOFTUART_T_INTCTL_REG      TIMSK

#define SOFTUART_CMPINT_EN_MASK    (1 << OCIE0A)

#define SOFTUART_CTC_MASKA         (1 << WGM01)
#define SOFTUART_CTC_MASKB         (0)
*/

/* "A timer interrupt must be set to interrupt at three times 
   the required baud rate." */
//#define SOFTUART_PRESCALE (8)
// #define SOFTUART_PRESCALE (1)
/*
#if (SOFTUART_PRESCALE == 8)
#define SOFTUART_PRESC_MASKA         (0)
#define SOFTUART_PRESC_MASKB         (1 << CS01)
#elif (SOFTUART_PRESCALE==1)
#define SOFTUART_PRESC_MASKA         (0)
#define SOFTUART_PRESC_MASKB         (1 << CS00)
#else 
#error "prescale unsupported"
#endif

#elif    defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__) 
      || defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644PA__)
      || defined (__AVR_ATmega164P__) || defined (__AVR_ATmega164A__)
*/
/*

#define SOFTUART_T_COMP_LABEL      TIMER0_COMPA_vect
#define SOFTUART_T_COMP_REG        OCR0A
#define SOFTUART_T_CONTR_REGA      TCCR0A
#define SOFTUART_T_CONTR_REGB      TCCR0B
#define SOFTUART_T_CNT_REG         TCNT0
#define SOFTUART_T_INTCTL_REG      TIMSK0
#define SOFTUART_CMPINT_EN_MASK    (1 << OCIE0A)
#define SOFTUART_CTC_MASKA         (1 << WGM01)
#define SOFTUART_CTC_MASKB         (0)
*/
/* "A timer interrupt must be set to interrupt at three times 
   the required baud rate." */
//#define SOFTUART_PRESCALE (8)
// #define SOFTUART_PRESCALE (1)
/*
#if (SOFTUART_PRESCALE == 8)
#define SOFTUART_PRESC_MASKA         (0)
#define SOFTUART_PRESC_MASKB         (1 << CS01)
#elif (SOFTUART_PRESCALE==1)
#define SOFTUART_PRESC_MASKA         (0)
#define SOFTUART_PRESC_MASKB         (1 << CS00)
#else 
#error "prescale unsupported"
#endif

#else
#error "no defintions available for this AVR"
#endif

*/
/*#define SOFTUART_TIMERTOP ( F_CPU/SOFTUART_PRESCALE/SOFTUART_BAUD_RATE/3 - 1)

#if (SOFTUART_TIMERTOP > 0xff)
#warning "Check SOFTUART_TIMERTOP: increase prescaler, lower F_CPU or use a 16 bit timer"
#endif
*/
#define SOFTUART_IN_BUF_SIZE     32

void interruptCAllback(void);

// Init the Software Uart
void softuart_init(void);

// Clears the contents of the input buffer.
void softuart_flush_input_buffer( void );

// Tests whether an input character has been received.
unsigned char softuart_kbhit( void );

// Reads a character from the input buffer, waiting if necessary.
char softuart_getchar( void );

// To check if transmitter is busy
unsigned char softuart_transmit_busy( void );

// Writes a character to the serial port.
void softuart_putchar( const char );

// Turns on the receive function.
void softuart_turn_rx_on( void );

// Turns off the receive function.
void softuart_turn_rx_off( void );

// Write a NULL-terminated string from RAM to the serial port
void softuart_puts( const char *s );

// Write a NULL-terminated string from program-space (flash) 
// to the serial port. example: softuart_puts_p(PSTR("test"))
void softuart_puts_p( const char *prg_s );

#define   pgm_read_byte(address_short)        pgm_read_byte_near(address_short)
#define   pgm_read_byte_near(address_short)   __LPM((uint16_t)(address_short))
// Helper-Macro - "automatically" inserts PSTR
// when used: include avr/pgmspace.h before this include-file
//#define softuart_puts_P(s___) softuart_puts_p(PSTR(s___))