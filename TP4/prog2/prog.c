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
		LATBbits.LATB1 = 1;
	}else if(c=='B' || c=='b'){
		LATBbits-LATB0 = 1;
	}else if(c=='C' || c=='c'){
		LATBbits.LATB6 = 1;
	}else if(c=='D' || c=='d'){
		LATBbits.LATB5 = 1;
	}else if(c=='E' || c=='e'){
		LATBbits.LATB4 = 1;
	}else if(c=='F' || c=='f'){
		LATBbits.LATB2 = 1;
	}else if(c=='G' || c=='g'){
		LATBbits.LATB3 = 1;
	}else if(c == '.'){
		LATBbits.LATB7 = 1;
	}
	
}

