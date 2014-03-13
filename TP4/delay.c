#define CALIBRATION_VALUE 4998

void delay(unsigned int n_intervals)
{
	volatile unsigned int a;
	for(; n_intervals > 0; n_intervals--){
		for(a=CALIBRATION_VALUE; a > 0; a--);
	}
}

