#include "detpic32.h"

void printHex(int value);

int main(){
	int i = 0;
	for (; i <= 16; ++i){
		printHex(i);
		putChar('\n');
	}
	return 0;
}

void printHex(int value){
	if(value>=0 && value<=9){
		printStr("0x");
		putChar('0'+value);
	}else if(value>=10 && value<16){
		printStr("0x");
		value -= 10;
		putChar('A'+value);
	}
}
