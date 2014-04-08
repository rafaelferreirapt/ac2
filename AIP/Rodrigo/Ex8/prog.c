#include <detpic32.h>

#define CALIBRATION_VALUE 4998

void delay(unsigned int);
void send2displays(unsigned char);

int main(void){
	int LENGHT = 20;
	int i;
	while(1){
		for(i=0;i<LENGHT;i++){
			int j = 0;
			do{
				send2displays(i);
				delay(10);
			}while(++j<49);
		}	
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
	static unsigned char displayFlag = 0;
	static const unsigned char display7Scodes[] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E};

	TRISB = (TRISB & 0xFC00);
	LATB = (LATB & 0xFC00);

	unsigned char digit_low = value & 0x0F;
	digit_low = display7Scodes[digit_low];
	unsigned char digit_high = value >> 4;
	digit_high = display7Scodes[digit_high];

	if(displayFlag == 0){
		LATBbits.LATB8 = 1;
		LATBbits.LATB9 = 0;
		LATB = (LATB & 0xFF00) | digit_high;
	}else{
		LATBbits.LATB8 = 0;
		LATBbits.LATB9 = 1;
		LATB = (LATB & 0xFF00) | digit_low;
	}

	displayFlag = !displayFlag;
}	

