#include <detpic32.h>
void send2display10(int n, int display){
	static const unsigned int codes[] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E};
	
	TRISB = TRISB & 0xFC00; /* configurar como saidas */
	LATB = LATB & 0xFC00; /* colocar tudo a 0 */
		int x = codes[n];
	if(display){
		LATBbits.LATB8 = 1;
		LATBbits.LATB9 = 0;
	}else{
		LATBbits.LATB8 = 0;
		LATBbits.LATB9 = 1;
	}
	LATB = (LATB & 0xFF00) | x;
}
