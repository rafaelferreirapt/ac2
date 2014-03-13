#include <detpic32.h>
void send2displays(unsigned char value){
	static unsigned char displayFlag = 0;
	static const unsigned char codes[] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E};

	TRISB = TRISB & 0xFC00; /* configurar como saidas */
	LATB = LATB & 0xFC00; /* colocar tudo a 0 => a mascara foi alterada para o bit 7 ser definido antes */

	unsigned char first = value & 0x0F;
	first = codes[first];
	unsigned char second = (value >> 4);
	second = codes[second];

	if (!displayFlag){
		LATBbits.LATB8 = 0;
		LATBbits.LATB9 = 1;
		LATB = (LATB & 0xFF00) | first;
		/* paridade
		if(first%2==0){
			LATB = (LATB & 0xFF00) | first | 0x80;
		}else{
			LATB = (LATB & 0xFF00) | first;
		}
		*/
	}else{
		LATBbits.LATB8 = 1;
		LATBbits.LATB9 = 0;
		LATB = (LATB & 0xFF00) | second;
		/* paridade
		if(first%2!=0){
			LATB = (LATB & 0xFF00) | second | 0x80;
		}else{
			LATB = (LATB & 0xFF00) | second;
		}
		*/
	}

	displayFlag = !displayFlag; /* toggle display */
}
