#include <detpic32.h>

int main(void)
{
	TRISBbits.TRISB14 = 1; // RB14 digital output disconnected
	AD1PCFGbits.PCFG14 = 0; // RB14 configured as analog input (AN14)

	AD1CHSbits.CH0SA = 14; // Selects AN14 as input for the A/D converter
	AD1CON2bits.SMPI = 0; // 4 samples will be converted and stored

}
