#include <detpic32.h>

int read7segments(){
	int num = 0;
	int binary[] = {PORTEbits.RE4, PORTEbits.RE5, PORTEbits.RE6, PORTEbits.RE7};
	int i = 0;
	for(; i<4; i++){
		if(binary[i]==1){
			int elevado = 1;
			int k = 0;
			for(; k<i; k++){
				elevado *= 2;	
			}
			num += elevado;
		}
	}
	return num;
}

