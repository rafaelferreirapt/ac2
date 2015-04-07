/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>

void delay(unsigned int);

int main(void)
{
	while(1)
	{
		resetCoreTimer();
		delay(1);
		printInt(readCoreTimer(), 10 + (10 << 16));
		printStr("\r\n");
	}
}
