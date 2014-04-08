#include <detpic32.h>;

/*
Exercício 2 - O segundo exercício é para contar de 0 a 20 hexadecimal
usando os displays para treinar como se configura (10 portos) quais
são os portos para ligar (8 e 9) o resto é para os códigos. Ver a função
send2displays porque pode ser pedida
*/

int CALIBRATION_VALUE = 4998;
void delay(int);
void send2displays(unsigned char);

void main(){
	int LENGTH = 20, i;
	while(1){
		for(i=0; i<LENGTH; i++){
			int j=0;
			do{
				send2displays(i);
				delay(10);
			}while(j++<50);
		}
	}
}

void delay(int num_intervals){
	volatile unsigned int i;
	for(; num_intervals != 0; num_intervals--)
		for(i = CALIBRATION_VALUE; i!=0; i--)
			;
}

void send2displays(unsigned char value){
	static unsigned char displayFlag = 0;
	static const unsigned char codes[] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E};
	/* eles dão-nos sempre estes códigos aqui de cima */

	TRISB = TRISB & 0xFC00;
	LATB = LATB & 0xFC00;

	unsigned char first = value & 0x0F;
	first = codes[first];
	unsigned char second = (value >> 4);
	second = codes[second];

	if (!displayFlag){
		LATBbits.LATB8 = 0;
		LATBbits.LATB9 = 1;
		LATB = (LATB & 0xFF00) | first;
	}else{
		LATBbits.LATB8 = 1;
		LATBbits.LATB9 = 0;
		LATB = (LATB & 0xFF00) | second;
	}

	displayFlag = !displayFlag;
}