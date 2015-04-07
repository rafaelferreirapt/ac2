/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>;

/*
Exercício 1 - Este exercício é básico, trata-se de
acender os leds como se fez nos últimos exercícios de treino de assembly.
*/

int CALIBRATION_VALUE = 4998;
void delay(int);

void main(){
	TRISE = (TRISE & 0xF0) | 0xF0;
	int array[] = {0x2, 0x1, 0x3, 0x4};
	int i;
	while(1){
		for(i = 0; i<4; i++){
			LATE = (LATE & 0xFFF0) | array[i];
			delay(100);
		}
	}
}

void delay(int num_intervals){
	volatile unsigned int i;
	for(; num_intervals != 0; num_intervals--){
		for(i = CALIBRATION_VALUE; i!=0; i--)
			;
	}
}
