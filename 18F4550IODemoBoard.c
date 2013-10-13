// 18F4550IODemoBoard.c

// chip config ////////////////////////////////////////////////////////////////////////////////////
					// clock options, see 18F4550 data sheet figure 2-1 "clock diagram" for explanation
#pragma config PLLDIV = 5				// this does not matter since we are using the internal clock & not using USB, see data sheet fig. 2-1
#pragma config CPUDIV = OSC1_PLL2		// this does not matter since we are using the internal clock & not using USB, see data sheet fig. 2-1
#pragma config USBDIV = 2				// this does not matter since we are using the internal clock & not using USB, see data sheet fig. 2-1

										// internal clock, pin #14 (RA6) as I/O pin, pin #13 unused,
#pragma config FOSC = INTOSCIO_EC		// if OSCCON is left as default clock speed will be 1Mhz

					// now the other less confusing options . . .
#pragma config FCMEN = OFF				// fail-safe clock monitor disabled
#pragma config IESO = OFF				// internal / external osc switchover bit disabled
#pragma config PWRT = OFF				// power-up timer disabled
#pragma config BOR = OFF				// brown-out reset disabled in hardware & software
#pragma config BORV = 3				// brown-out reset voltage bits, does not matter since brown-out is disabled 
#pragma config VREGEN = OFF			// USB voltage regulator, would need to turn on if using internal USB voltage regulator for USB comm.
#pragma config WDT = OFF				// watchdog timer disabled
#pragma config WDTPS = 32768			// watchdog timer postscale, does not matter since watchdog timer is disabled
#pragma config CCP2MX = ON			// use RC1 (pin #16) as CCP2 MUX (this is the default pin for CCP2 MUX)
#pragma config PBADEN = OFF			// RB0, RB1, RB2, RB3, & RB4 are configured as digital I/O on reset
#pragma config LPT1OSC = OFF			// disable low-power option for timer 1 (timer 1 in regular mode)
#pragma config MCLRE = OFF				// master clear disabled, pin #1 is for VPP and / or RE3 use
#pragma config STVREN = ON				// stack full/underflow will cause reset
#pragma config LVP = OFF				// single-supply ICSP disabled
#pragma config ICPRT = OFF				// in-circuit debug/programming port (ICPORT) disabled, this feature is not available on 40 pin DIP package
#pragma config XINST = OFF				// instruction set extension and indexed addressing mode disabled (this is the default setting)
#pragma config DEBUG = OFF				// background debugger disabled, RA6 & RB7 configured as general purpose I/O pins
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF			// code protection bits off
#pragma config CPB = OFF				// boot block code protection off
#pragma config CPD = OFF				// data EEPROM code protection off
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF		// write protection bits off
#pragma config WRTC = OFF				// config registers write protection off
#pragma config WRTB = OFF				// boot block is not write protected
#pragma config WRTD = OFF				// data EEPROM is not write protected
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF	// table read protection bits off
#pragma config EBTRB = OFF				// boot block table read protection off

#include<p18f4550.h>

// defines /////////////////////////////////////////////////////////////////////////////////////////
typedef unsigned char BYTE;

#define POT0	PORTAbits.RA0
#define SW1		PORTAbits.RA1
#define SW2		PORTAbits.RA2
#define SW3		PORTAbits.RA3

#define LED0 PORTBbits.RB3
#define LED1 PORTBbits.RB2
#define LED2 PORTBbits.RB1
#define LED3 PORTBbits.RB0
#define LED4 PORTDbits.RD7
#define LED5 PORTDbits.RD6
#define LED6 PORTDbits.RD5
#define LED7 PORTDbits.RD4

// function prototypes ////////////////////////////////////////////////////////////////////////////
void showByteOnLEDs(BYTE byteToShow);

///////////////////////////////////////////////////////////////////////////////////////////////////
void main(void) {
	OSCCONbits.IRCF2 = 1;		// set internal clock to 8 MHz
	OSCCONbits.IRCF1 = 1;		//
	OSCCONbits.IRCF0 = 1;		//
	
	TRISAbits.RA0 = 1;			// set pins for pot & switches to input
	TRISAbits.RA1 = 1;			//
	TRISAbits.RA2 = 1;			//
	TRISAbits.RA3 = 1;			//
	
	TRISBbits.RB3 = 0;			// set pins for LEDs 0 - 3 to input
	TRISBbits.RB2 = 0;			//
	TRISBbits.RB1 = 0;			//
	TRISBbits.RB0 = 0;			//
	
	TRISDbits.RD7 = 0;			// set pins for LEDs 4 - 7 to input
	TRISDbits.RD6 = 0;			//
	TRISDbits.RD5 = 0;			//
	TRISDbits.RD4 = 0;			//
	
	ADCON0bits.CHS3 = 0;		// set analog channel to AN0 / RA0
	ADCON0bits.CHS2 = 0;		//
	ADCON0bits.CHS1 = 0;		//
	ADCON0bits.CHS0 = 0;		//
	
	ADCON1bits.VCFG1 = 0;		// set A/D converter negative reference to gnd
	ADCON1bits.VCFG0 = 0;		// set A/D converter positive reference to pwr
	
	ADCON1bits.PCFG3 = 1;		// set RA0 / AN0 to analog input, all other pins digital input
	ADCON1bits.PCFG2 = 1;		//
	ADCON1bits.PCFG1 = 1;		//
	ADCON1bits.PCFG0 = 0;		//
	
	ADCON2bits.ADFM = 0;		// left justify A/D result, "8 bit" A/D result will be in reg ADRESH
	
	ADCON2bits.ACQT2 = 1;		// set A/D acq time
	ADCON2bits.ACQT1 = 0;		//
	ADCON2bits.ACQT0 = 1;		//
	
	ADCON2bits.ADCS2 = 1;		// set A/D conversion clock
	ADCON2bits.ADCS1 = 0;		//
	ADCON2bits.ADCS0 = 1;		//
	
	ADCON0bits.ADON = 1;		// turn A/D converter on
	
	while(1) {
		if(SW1 == 1) {				// if SW1 is off (logic low) => digital mode
			if(SW2 == 0) {			// if SW2 is on (logic low)
				LED0 = 1;			// turn LEDs 0 - 3 on
				LED1 = 1;
				LED2 = 1;
				LED3 = 1;
			} else if(SW2 == 1) {		// else if SW2 is off (logic low)
				LED0 = 0;			// turn LEDs 0 - 3 off
				LED1 = 0;
				LED2 = 0;
				LED3 = 0;
			} else {
				// should never get here
			}
			if(SW3 == 0) {			// if SW3 is on (logic low)
				LED4 = 1;			// turn LEDs 4 - 7 on
				LED5 = 1;
				LED6 = 1;
				LED7 = 1;
			} else if(SW3 == 1) {		// else if SW3 is off (logic low)
				LED4 = 0;			// turn LEDs 4 - 7 off
				LED5 = 0;
				LED6 = 0;
				LED7 = 0;
			} else {
				// should never get here
			}
		}
		else if(SW1 == 0) {			// if SW1 is on (logic low) => analog mode
			
			ADCON0bits.GO_DONE = 1;					// perform A/D conversion
			while(ADCON0bits.GO_DONE == 1) { }			// wait here until ADCONbits.GO_DONE == 0
			
			showByteOnLEDs(ADRESH);
			
		} else {
			// should never get here
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void showByteOnLEDs(BYTE byteToShow) {
	if((byteToShow & 0b10000000) == 0b10000000) LED7 = 1;
	else LED7 = 0;
	
	if((byteToShow & 0b01000000) == 0b01000000) LED6 = 1;
	else LED6 = 0;
	
	if((byteToShow & 0b00100000) == 0b00100000) LED5  = 1;
	else LED5  = 0;
	
	if((byteToShow & 0b00010000) == 0b00010000) LED4  = 1;
	else LED4  = 0;
	
	if((byteToShow & 0b00001000) == 0b00001000) LED3  = 1;
	else LED3  = 0;
	
	if((byteToShow & 0b00000100) == 0b00000100) LED2  = 1;
	else LED2  = 0;
	
	if((byteToShow & 0b00000010) == 0b00000010) LED1  = 1;
	else LED1  = 0;
	
	if((byteToShow & 0b00000001) == 0b00000001) LED0  = 1;
	else LED0  = 0;
}
