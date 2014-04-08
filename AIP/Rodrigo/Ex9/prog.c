#include <detpic32.h>

#define CALIBRATION_VALUE 4998

void delay(unsigned int);
void send2displays(unsigned char);

int main(void){
	TRISE = (TRISE & 0xFFF0) | 0x00F0;

	//configure ADC

	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG4 = 0;

	AD1CON1bits.SSRC = 7;

	AD1CON1bits.CLRASAM = 1;

	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 3;

	AD1CHSbits.CH0SA = 14;

	AD1CON1bits.ON = 1;

	IFS1bits.AD1IF = 0;
	IPC6bits.AD1IP = 2;
	IEC1bits.AD1IE = 1;

	EnableInterrupts();

	AD1CON1bits.ASAM = 1;

	while(1){

	}

}

void _int_(27) isr_adc(void){
	int media = 0;
	int i;
	int aux, aux1;

	int *p = (int *)(&ADC1BUF0);

	for(i = 0; i<4; i++){
		media += p[i*4];
	}

	media /= 4;
	aux = PORTEbits.RE7;
	aux = aux << 1;
	aux = aux | PORTEbits.RE6;
	aux = aux << 1;
	aux = aux | PORTEbits.RE5;
	aux = aux << 1;
	aux = aux | PORTEbits.RE4;
	
	aux1 = media - aux;
	printInt(media, 16);
	putChar('\n');
	printInt(aux, 16);
	putChar('\n');
	printInt(aux1, 16);
	putChar('\n');

	if(media >= 3*0xFF){
		LATE = (LATE & 0xFFF0) | aux; 
	}

	do{
		send2displays(aux1);
		delay(10);
	}while(++i<100);

	IFS1bits.AD1IF = 0;
	AD1CON1bits.ASAM = 1;
}

void delay(unsigned int n_intervals){
	volatile unsigned int i;

	for(; n_intervals != 0; n_intervals--)
		for(i=CALIBRATION_VALUE; i!=0; i--)
			;
}

void send2displays(unsigned char value){
	static unsigned char displayFlag = 0;
	static const unsigned char display7Scodes[] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E};

	TRISB = (TRISB & 0xFC00);
	LATB = (LATB & 0xFC00);

	unsigned char digit_low = value & 0x0F;
	digit_low = display7Scodes[digit_low];
	unsigned char digit_high = value >> 4;
	digit_high = display7Scodes[digit_high];

	if(displayFlag == 0){
		LATBbits.LATB8 = 0;
		LATBbits.LATB9 = 1;
		LATB = (LATB & 0xFF00) | digit_low;
	}else{
		LATBbits.LATB9 = 0;
		LATBbits.LATB8 = 1;
		LATB = (LATB & 0xFF00) | digit_high;
	}

	displayFlag = !displayFlag;

}
