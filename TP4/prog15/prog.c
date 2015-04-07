/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>
void send2displays(char);
void delay(unsigned int);
unsigned char toBcd(unsigned char);

int main(void){
	unsigned char c = 0, tmp;
	int i;

	while(1){
		i = 0;
		do{
			delay(10);
			tmp = toBcd(c);
			send2displays(tmp);
		}while(++i<4);
		if(c==0x3B){
			c = 0;
			do{
				i = 0;
				do{
					delay(10);
					send2displays(0);
				}while(++i<50);
				/* limpar o display */
				LATB = LATB & 0xFC00; /* colocar tudo a 0 => a mascara foi alterada para o bit 7 ser definido antes */
				/* esperar 500 ms */
				delay(500);
			}while(++c<5);
			c = 0;
		}else{
			c++;
		}
	};
	return 0;
}
