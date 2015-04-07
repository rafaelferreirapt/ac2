/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include <detpic32.h>
#include "i2c.h"

int getTemperature(int *temperature){
	int ack;

	i2c1_init(TC74_CLK_FREQ);

	// Send Start event
	i2c1_start();

	// Send Address + WR (ADDR_WR) and copy return value to "ack" variable ...(see exercise 7, function main())...
	ack = i2c1_send(ADDR_WR);

	ack += i2c1_send(RTR);

	i2c1_start();

	ack += i2c1_send(ADDR_RD);

	*temperature = i2c1_receive(I2C_NACK);

	// Send Stop event
	i2c1_stop();

	return ack;
}

int main(void){
	while(1){
		int temperature;

		if(getTemperature(&temperature) == 0){
			printInt10(temperature);
			printStr("\e[G");
		}

		delay(250);
	}
	return 0;
}
