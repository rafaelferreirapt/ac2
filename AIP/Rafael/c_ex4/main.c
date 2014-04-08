#include <detpic32.h>

void delay(unsigned int);
void send2displays(unsigned char);
int CALIBRATION_VALUE = 4998;

int main(){
	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0;
	AD1CHSbits.CH0SA = 14;

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 3;
	AD1CON1bits.ON = 1;

	IPC6bits.AD1IP = 4;
	IEC1bits.AD1IE = 1;
	EnableInterrupts();


	IFS1bits.AD1IF = 0;
	AD1CON1bits.ASAM = 1;

	while(1){

	}

	return 0;
}

void delay(unsigned int num_intervals){
	volatile unsigned int i;
	for(; num_intervals != 0; num_intervals--)
		for(i = CALIBRATION_VALUE; i!=0; i--)
			;
}

void send2displays(unsigned char value){
	static const unsigned char codes[] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43,
		0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E};
	static unsigned int flag = 1;

	TRISB = TRISB & 0xFC00;
	LATB = LATB & 0xFC00;

	unsigned int first = codes[(value & 0x0F)];
	unsigned int second = codes[(value >> 4)];

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

void _int_(27) isr_adc(void){
	unsigned int i = 0, media = 0;
	int *p = (int *)(&ADC1BUF0);

	for(; i<4; i++){
		media += p[i*4];
	}

	media /= 4;
	i = 0;

	if(media >= 0x3FE){
		TRISE = TRISE & 0xF0;
		LATE = (LATE & 0xF0) | 0x0F;
	}

	do{
		send2displays(media);
		delay(10);
		if(media >= 0x3FE && i>=50){
			LATE = (LATE & 0xF0);
		}
	}while(++i<100);

	IFS1bits.AD1IF = 0;
	AD1CON1bits.ASAM = 1;
}
