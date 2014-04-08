#include <detpic32.h>

#define CALIBRATION_VALUE 4998
void delay(unsigned int);

int main(void){
	//configuraçao saidas
	TRISEbits.TRISE0 = 0;
	TRISEbits.TRISE1 = 0;
	TRISEbits.TRISE2 = 0;
	TRISEbits.TRISE3 = 0;
	//configuraçao saidas

	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0;

	AD1CHSbits.CH0SA = 14;
	AD1CON2bits.SMPI = 3;

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;

	AD1CON1bits.ON = 1;

	
	while(1){
		
		IFS1bits.AD1IF = 0;
		AD1CON1bits.ASAM = 1;
		while(IFS1bits.AD1IF == 0);
		int media=0; 
		int i;
		int *p = (int *)(&ADC1BUF0);

		for(i =0; i<4; i++){
			media += p[i*4];
		}

		media /= 4;

		if(media >= 3*0xFF){
			LATE = (LATE & 0xFFF0) | 0x000F;
			delay(100);
			LATE = (LATE & 0xFFF0);
			delay(100);
		}else if(media >= 2*0xFF){
			LATE = (LATE & 0xFFF0) | 0x0003;
		}else if(media >= 0xFF){
			LATE = (LATE & 0xFFF0) | 0x0001;
		}else{
			LATE = (LATE & 0xFFF0); 
		}
		
	}

	return 0;
}


void delay(unsigned int n_intervals){
	volatile unsigned int i;

	for( ;n_intervals != 0; n_intervals--)
		for (i = CALIBRATION_VALUE; i != 0; i--)
			;
}

