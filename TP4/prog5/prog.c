#include <detpic32.h>

void delay(unsigned int);
void send2display10(int, int);

int main(void){
	int display = 0;
	while(1){
		int i;
		for(i=0; i<16; i++){
			send2display10(i, display);
			delay(1000);
		}
		display = !display;
	}
}

