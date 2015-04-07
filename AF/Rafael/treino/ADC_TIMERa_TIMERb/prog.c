/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
/*
Lê do dip-switch 2 bits, e consoante a codificação binária faz:
    * 00: um contador binário decrescente é emitido nos leds (4 leds) a uma frequência de 20Hz imposta pelo timer do relógio do tipo A (se o relógio do tipo B estiver ligado deve desligá-lo);
    * 01: um contador binário crescente com uma frequência de 30Hz imposta pelo relógio do tipo B (se o relógio do tipo A estiver ligado deve desligá-lo);
    * 10: faz o freeze do último valor do contador
    * 11: deve desligar os led’s se a ADC for mínimo e deve ligar os leds se a ADC for máximo, fazendo o output no terminal;
*/
#include "detpic32.h"
#define SAMPLES 8

int contador = 0, ledsOn = 1;

int main(){
	/* configurar a leitura do dipswitch */
	TRISEbits.TRISE4 = 1;
	TRISEbits.TRISE5 = 1;

	/* configurar os leds e limpá-los */
	TRISEbits.TRISE0 = 0;
	TRISEbits.TRISE1 = 0;
	TRISEbits.TRISE2 = 0;
	TRISEbits.TRISE3 = 0;

	LATEbits.LATE0 = 0;
	LATEbits.LATE1 = 0;
	LATEbits.LATE2 = 0;
	LATEbits.LATE3 = 0;

	/* config timer a(1), freq 20 Hz */
	T1CONbits.TCKPS = 2;
	PR1 = 15624;
	TMR1 = 0;

	IFS0bits.T1IF = 0;
	IPC1bits.T1IP = 4;
	IEC0bits.T1IE = 1;

	/* config timer b(2), freq 30Hz */
	T2CONbits.TCKPS = 4;
	PR2 = 41666;
	TMR2 = 0;

	IFS0bits.T2IF = 0;
	IPC2bits.T2IP = 4;
	IEC0bits.T2IE = 1;

	/* config ADC */
	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0;

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = SAMPLES-1;
	AD1CHSbits.CH0SA = 14;

	IPC6bits.AD1IP = 3;
	IEC1bits.AD1IE = 1;
	IFS1bits.AD1IF = 0;

	EnableInterrupts();

	while(1){
		if(!T1CONbits.TON && !PORTEbits.RE4 && !PORTEbits.RE5){
			/* contador freq 20Hz */
			if(T2CONbits.TON){
				T2CONbits.TON = 0;
			}
			T1CONbits.TON = 1;
			ledsOn = 1;
		}else if(!T2CONbits.TON && !PORTEbits.RE4 && PORTEbits.RE5){
			/* contador freq 30Hz */
			if(T1CONbits.TON){
				T1CONbits.TON = 0;
			}
			T2CONbits.TON = 1;
			ledsOn = 1;
		}else if(PORTEbits.RE4 && !PORTEbits.RE5){
			/* freeze do último valor */
			if(T1CONbits.TON){
				T1CONbits.TON = 0;
			}
			if(T2CONbits.TON){
				T2CONbits.TON = 0;
			}
		}else if(PORTEbits.RE4 && PORTEbits.RE5){
			/* start ADC */
			AD1CON1bits.ON = 1;
			AD1CON1bits.ASAM = 1;
		}
	}

	return 0;
}

void _int_(27) isr_adc(void){
	/* sempre que existir novos valores na adc vai ver se o valor é perto do máximo e se sim deve ligar os leds, se for minimo deve desligar os leds */
	int *p = (int *)(&ADC1BUF0), media = 0, i;
    for( i = 0; i < SAMPLES; i++ ){
    	media += p[i*4];
	}
	media /= SAMPLES;
	if(media>0x300){
		ledsOn = 1;
	}else if(media<0x0F0){
		ledsOn = 0;
	}
	//printInt(media, 0x00030010);
	//putChar('\n');
	/* a adc está com problemas */
	IFS1bits.AD1IF = 0;
}

void ledsCounter(){
	if(ledsOn){
		LATE = (LATE & 0xF0) | contador;
	}
}

void incrementCounter(){
	contador = ((contador+1) & 0x0F);
}

void _int_(4) isr_t1(void){
	incrementCounter();
	ledsCounter();
	IFS0bits.T1IF = 0;
}

void _int_(8) isr_t2(void){
	incrementCounter();
	ledsCounter();
	IFS0bits.T2IF = 0;
}
