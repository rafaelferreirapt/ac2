#include "detpic32.h"

int contador = 0, flagT2 = 1, contadorLedsEnable=0;

void dutyCycle(int duty){
	OC1RS = ((PR3 + 1) * duty)/100;
}

void ocrNormal(){
	OC1CONbits.ON = 1;
}

void startCounter(){
	T2CONbits.TON = 1;
}

void displayTimer(){
	T4CONbits.TON = 1;
}

void turnOffLedsOC1(){
	OC1CONbits.ON = 0;
	contadorLedsEnable = 0;
	LATE = (LATE & 0xF0);
}

void showLeds(){
	LATE = (LATE & 0xF0) | (contador & 0x0F);
}

void _int_(8) isr_T2(void){
	if(flagT2){
		contador = ((contador+1) & 0x0F);
		flagT2 = 0;

		if(contadorLedsEnable){
			showLeds();
		}
	}else{
		flagT2 = 1;
	}
	IFS0bits.T2IF = 0;
}

void _int_(16) isr_T4(void){
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
	IFS0bits.T4IF = 0;
}

int main(void){
	/* FAZER TRIGGER PARA ENTRADAS */
	TRISE = (TRISE | 0xF0);

	/* configurar os leds como saida */
	TRISEbits.TRISE0 = 0;
	TRISEbits.TRISE1 = 0;
	TRISEbits.TRISE2 = 0;
	TRISEbits.TRISE3 = 0;
	LATE = (LATE & 0xF0);

	/* configurar o timer 2 */
	T2CONbits.TCKPS = 7;
	PR2 = 39062;
	TMR2 = 0;

	IFS0bits.T2IF = 0;
	IPC2bits.T2IP = 6;
	IEC0bits.T2IE = 1;

	/* configurar o timer 3 */
	T3CONbits.TCKPS = 0;
	PR3 = 40000;
	TMR3 = 0;
	T3CONbits.TON = 1;

	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 1;
	dutyCycle(60);

	/* configurar o timer 4 */
	T4CONbits.TCKPS = 0;
	PR4 = 39000;
	TMR4 = 0;

	IFS0bits.T4IF = 0;
	IPC4bits.T4IP = 3;
	IEC0bits.T4IE = 1;

	EnableInterrupts();

	while(1){
		if(!OC1CONbits.ON && PORTEbits.RE4){
			ocrNormal();
		}
		if(!contadorLedsEnable && PORTEbits.RE5){
			contadorLedsEnable = 1;
			startCounter();
		}
		if(!T4CONbits.TON && PORTEbits.RE6){
			startCounter();
			displayTimer();
		}
		if(PORTEbits.RE7){
			turnOffLedsOC1();
		}
	}

	return 0;
}