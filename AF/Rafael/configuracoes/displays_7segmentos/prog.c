/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include "detpic32.h"

int sequencia[] = {0x01, 0x02, 0x4, 0x6, 0x8,
				   0x10, 0x12, 0x16,0x17,0x20};
int modulo = 10, contador = 0, flagT2=1;

int main(){
	/* configuração do timer 2 */
	/* este timer vai ter associado o contador, deve portanto ter
	   5Hz */
	T2CONbits.TCKPS = 6;
	PR2 = 62499;
	TMR2 = 0;
	T2CONbits.TON = 1;

	IFS0bits.T2IF = 0;
	IPC2bits.T2IP = 6;
	IEC0bits.T2IE = 1;

	/* configuração do timer 3 */
	/* refrescamento dos leds a 20Hz */
	T3CONbits.TCKPS = 0;
	PR3 = 39000;
	TMR3 = 0;
	T3CONbits.TON = 1;

	IFS0bits.T3IF = 0;
	IPC3bits.T3IP = 4;
	IEC0bits.T3IE = 1;

	EnableInterrupts();
	while(1);
	return 0;
}

void send2displays(unsigned char value){
	static const unsigned char codes[] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43,
		0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E};
	static unsigned int flag = 1;

	TRISB = TRISB & 0xFC00;
	LATB = LATB & 0xFC00;

	unsigned int first = codes[(contador & 0x0F)];
	unsigned int second = codes[(contador >> 4)];

	if(flag){
		LATBbits.LATB9 = 0;
		LATBbits.LATB8 = 1;
		LATB = (LATB & 0xFF00) | first;
	}else{
		LATBbits.LATB9 = 1;
		LATBbits.LATB8 = 0;
		LATB = (LATB & 0xFF00) | second;
	}

	flag = !flag;
}

void _int_(8) isr_T2(void){
	if(flagT2){
		if(contador>=modulo){
			contador=0;
		}else{
			contador++;
		}
		flagT2 = 0;
	}else{
		flagT2 = 1;
	}

	IFS0bits.T2IF = 0;
}

void _int_(12) isr_T3(void){
	send2displays(sequencia[contador]);
	IFS0bits.T3IF = 0;
}
