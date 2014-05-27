/*
Usando uma variável percentage_duty de 0 a 100 implemente um duty cycle no timer B de 20Hz. Que incrementa com + 5, e -5 usando a UART com interrupção de recepção.
Usando o dipswitch tendo a combinação binária 11 deve fazer freeze do valor e com outros deve deixar mudar o valor do dutycycle o OCR.
Usando os leds deve indicar qual o valor em barra de leds do duty cycle (perecntage), sendo 0 tudo apagado e 100 tudo ligado.
Usando a ADC deve activar para o valor máximo os leds todos a piscar com o timer do tipo A a 25Hz.
Nos displays deve indicar a percentagem da ADC.
*/

#include "detpic32.h"
#define SAMPLES 4

int duty_cycle_percentage = 0, flag = 1, adc_percentage = 0;

void send2displays(){
	static const unsigned char codes[] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43,
										  0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E};
	static unsigned int flag = 1;

	TRISB = TRISB & 0xFC00;
	LATB = LATB & 0xFC00;

	unsigned int first 	= codes[(adc_percentage & 0x0F)];
	unsigned int second = codes[adc_percentage>>4];

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

void changeLeds(){
	if(!T1CONbits.TON){
		if(duty_cycle_percentage==100){
			LATE = (LATE & 0xF0) | 0x0F;
		}else if(duty_cycle_percentage>=75){
			LATEbits.LATE0 = 1;
			LATEbits.LATE1 = 1;
			LATEbits.LATE2 = 1;
		}else if(duty_cycle_percentage>=50){
			LATEbits.LATE0 = 1;
			LATEbits.LATE1 = 1;
		}else if(duty_cycle_percentage>=25){
			LATEbits.LATE0 = 1;
		}else{
			LATE = (LATE & 0xF0);
		}
	}else{
		if(flag){
			LATE = (LATE & 0xF0) | 0x0F;
			flag = 0;
		}else{
			LATE = (LATE & 0xF0);
			flag = 1;
		}
	}
}

void dutycycle(){
	if(PORTEbits.RE4!=1 || PORTEbits.RE5!=1){
		OC1RS = ((PR2 + 1)*duty_cycle_percentage)/100;
	}
	changeLeds();
}

int main(){
	/* timer do tipo B => 20Hz, duty cycle */
	T2CONbits.TCKPS = 4;
	PR2 = 62499;
	TMR2 = 0;
	T2CONbits.TON = 1;

	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 0;
	dutycycle();
	OC1CONbits.ON = 1;

	/* UART configuration */
	U1BRG = ((20000000+8*115200)/(16*115200))-1;
	U1MODEbits.BRGH = 0;
	U1MODEbits.PDSEL = 0b00;
	U1MODEbits.STSEL = 0;

	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;

	U1STAbits.URXISEL = 0b00;
	IEC0bits.U1RXIE = 1;
	IPC6bits.U1IP = 3;
	IPC6bits.U1IP = 3;

	/* configurar dipswitch */
	TRISEbits.TRISE4 = 1;
	TRISEbits.TRISE5 = 1;

	/* configurar os leds */
	TRISE = (TRISE & 0xF0);
	LATE = (LATE & 0xF0);

	/* configurar timer tipo A */
	T1CONbits.TCKPS = 2;
	PR1 = 12499;
	TMR1 = 0;

	IFS0bits.T1IF = 0;
	IPC1bits.T1IP = 5;
	IEC0bits.T1IE = 1;

	/* config da UART */
	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0;

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = SAMPLES-1;
	AD1CHSbits.CH0SA = 14;
	AD1CON1bits.ON = 1;

	IFS1bits.AD1IF = 0;
	IPC6bits.AD1IP = 6;
	IEC1bits.AD1IE = 1;
	AD1CON1bits.ASAM = 1;

	/* configurar o timer 4 */
	T4CONbits.TCKPS = 0;
	PR4 = 39000;
	TMR4 = 0;
	T4CONbits.TON = 1;

	IFS0bits.T4IF = 0;
	IPC4bits.T4IP = 6;
	IEC0bits.T4IE = 1;

	EnableInterrupts();

	while(1){
	}

	return 0;
}

void _int_(24) isr_uart1(void){
	if(U1RXREG=='+'){
		duty_cycle_percentage += 5;
		if(duty_cycle_percentage>100){
			duty_cycle_percentage = 100;
		}
		dutycycle();
	}else if(U1RXREG=='-'){
		duty_cycle_percentage -= 5;
		if(duty_cycle_percentage<0){
			duty_cycle_percentage = 0;
		}
		dutycycle();
	}
	IFS0bits.U1RXIF = 0;
}

void _int_(27) isr_adc(void){
	int *p = (int *)(&ADC1BUF0);
	int i, media = 0;

	for(i=0; i<SAMPLES; i++){
		media += p[i*4];
	}

	media /= SAMPLES;

	if(media > 0x299){
		T1CONbits.TON = 1;
		OC1CONbits.ON = 0;
	}else{
		T1CONbits.TON = 0;
		OC1CONbits.ON = 1;
		LATE = (LATE & 0xF0);
	}

	adc_percentage = (media*100)/1023;

	printInt10(adc_percentage);
	putChar('\n');

	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;
}

void _int_(4) isr_T1(void){
	changeLeds();
	IFS0bits.T1IF = 0;
}

void _int_(16) isr_T4(void){
	send2displays();
	IFS0bits.T4IF = 0;
}
