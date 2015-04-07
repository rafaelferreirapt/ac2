/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>

void delay(unsigned int);

int main(void){
	static const unsigned int codes[] = {0x02, 0x01, 0x40, 0x20, 0x10, 0x04, 0x08, 0x80};

	TRISB = TRISB & 0xfc00; /* configurar tudo como saidas */
	LATB = LATB & 0Xfc00; /* desligar tudo os leds */
	LATBbits.LATB8 = 0; /* desligar o high */
	LATBbits.LATB9 = 1; /* ligar o low */

	while(1){
		int i;
		for(i=0; i<8; i++){
			int x = codes[i];
			LATB = (LATB & 0xFF00) | x;
			delay(1000);
		}
		LATBbits.LATB8 = !LATBbits.LATB8 ;
		LATBbits.LATB9 = !LATBbits.LATB9;
	}
}
