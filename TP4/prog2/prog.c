#include <detpic32.h>

void displayChar(char);

int main(void){
	TRISB = TRISB & 0xFC00; /* 1111 1100 0000 0000 */
	
	LATBbits.LATB9 = 1;
	LATBbits.LATB8 = 0;
	
	char c;
	
}

void displayChar(char c){
	LATB = LATB & 0xFF00; /* 1111 1111 0000 0000 => desliga o led */
	/* A => RB1 , B => RB0 , C => RB6 , D => RB5 , E => RB4, 
	   F => RB2 */
	if(c=='A' || c=='a'){

	}else if(c=='B' || c=='b'){

	}else if(c=='' || c==''){
	
}

