#include <detpic32.h>

#define SAMPLES 4

void putc(unsigned char);
void puts(unsigned char *string);
void configUart(unsigned int, char, unsigned int);
void setPWM(unsigned int);
void send2displays(unsigned char);

unsigned int value2display, voltMin = 55, voltMax;

void configADCandTIMER3(){
	/*** Configure ADC ***/
	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0;

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = SAMPLES - 1;
	AD1CHSbits.CH0SA = 14;
	AD1CON1bits.ON = 1;

	IPC6bits.AD1IP = 3;
	IEC1bits.AD1IE = 1;
	IFS1bits.AD1IF = 0;

	/*** Configure Timer 3 ***/
	T3CONbits.TCKPS = 2;
	PR3 = 50000;
	TMR3 = 0;
	T3CONbits.TON = 1;

	IFS0bits.T3IF = 0;
	IPC3bits.T3IP = 2;
	IEC0bits.T3IE = 1;
}

int main(void){

	configUart(115200,'N',1);
	configADCandTIMER3();

	/*** OCM **/
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 1;
	OC1CONbits.ON = 1;

	EnableInterrupts();

	AD1CON1bits.ASAM = 1;

	while(1){
		if(PORTEbits.RE4 == 0 && PORTEbits.RE5 == 0){
			setPWM(0);
			IEC0bits.T1IE = 1;
		}else if(PORTEbits.RE4 == 0 && PORTEbits.RE5 == 1){
			setPWM(100);
			IEC0bits.T1IE = 0;
		}else if(PORTEbits.RE4 == 1 && PORTEbits.RE5 == 0){
			if(PORTEbits.RE7 == 0 && PORTEbits.RE6 == 0){
				setPWM(3);
				send2displays(3);
			}else if(PORTEbits.RE7 == 0 && PORTEbits.RE6 == 1){
				setPWM(15);
				send2displays(15);
			}else if(PORTEbits.RE7 == 1 && PORTEbits.RE6 == 0){
				setPWM(40);
				send2displays(40);
			}else if(PORTEbits.RE7 == 1 && PORTEbits.RE6 == 1){
				setPWM(90);
				send2displays(90);
			}
			IEC0bits.T1IE = 0;
		}
	}

	while(1);
}

void _int_(24) isr_uart1(void){
	if(U1RXREG=='l'){
		puts("voltMin: ");
		printInt10(voltMin);
		puts(" voltMax: ");
		printInt10(voltMax);
		putc('\n');
	}
	IFS0bits.U1RXIF = 0;
}

void _int_(27) isr_adc(void){
	int i, v = 0, *p = (int *)(&ADC1BUF0);

	for(i=0; i<SAMPLES; i++){
		v += (p[i*4]*33+511)/1023;
	}

	v /= SAMPLES;
	value2display = v;

	if(v > voltMax){
		voltMax = v;
	}
	if(v <= voltMin){
		voltMin = v;
	}

	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;
}

void _int_(12) isr_T3(void){
	static int counter = 0;
	send2displays(value2display);
	if(++counter==100){
		counter = 0;
		puts("- Voltage: ");
		printInt10(value2display);
		putc('\n');
	}
	IFS0bits.T3IF = 0;
}

void configUart(unsigned int baud, char parity, unsigned int Stopbits){
	if(baud<600 || baud > 115200){
		baud = 115200;
	}
	if(parity!='N' && parity!='E' && parity!='O'){
		parity = 'N';
	}
	if(Stopbits!=1 && Stopbits!=2){
		Stopbits = 1;
	}

	U1BRG = ((20000000 + 8*baud)/ (16*baud))-1;
	U1MODEbits.BRGH = 0;

	if(parity=='N'){
		U1MODEbits.PDSEL = 0;
	}else if(parity=='E'){
		U1MODEbits.PDSEL = 1;
	}else if(parity=='O'){
		U1MODEbits.PDSEL = 0b10;
	}

	U1MODEbits.STSEL = (--Stopbits);
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;

	/*** UART configure interrupts ***/
	U1STAbits.URXISEL = 00;
	IEC0bits.U1RXIE = 1;
	IPC6bits.U1IP=3;
	IEC0bits.U1EIE = 1;
}


void putc(unsigned char byte2send){
	while(U1STAbits.UTXBF);
	U1TXREG = byte2send;
}

void puts(unsigned char *string){
	while(string[0]!='\0'){
		putc(string[0]);
		string++;
	}
}

void setPWM(unsigned int dutyCycle){
	OC1RS = ((PR3+1)*dutyCycle)/100;
}
