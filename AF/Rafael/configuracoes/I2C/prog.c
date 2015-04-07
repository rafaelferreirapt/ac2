/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
#include "detpic32.h"

#define  I2C_READ		1
#define  I2C_WRITE		0
#define  I2C_ACK 		0
#define  I2C_NACK 		1

#define  ADDR_WR		((SENS_ADDRESS << 1) | I2C_WRITE)
#define  ADDR_RD		((SENS_ADDRESS << 1) | I2C_READ)
#define  SENS_ADDRESS 	0x4D
#define  TC74_CLK_FREQ	100000
#define  RTR			0

void delay(unsigned int);

int main(){
	int ack, temperature;
   	i2c1_init(TC74_CLK_FREQ);

   	while(1){
   		i2c1_start();
		ack = i2c1_send(ADDR_WR);
   		i2c1_start();
   		ack += i2c1_send(RTR);
		i2c1_start();
		ack += i2c1_send(ADDR_RD);
		if(ack != 0){
			i2c1_stop();
			printStr("Error \n");
			break;
		}
		temperature = i2c1_receive(I2C_NACK);
		i2c1_stop();
		printInt10(temperature);
		printStr("\e[G");
		delay(250);
   	}
	return 0;
}

void i2c1_init(unsigned int clock_freq){
	I2C1BRG = (20000000+clock_freq) / (2*clock_freq)-1;
	I2C1CONbits.ON = 1;
}

void i2c1_start(void){
	I2C1CONbits.SEN = 1;
	while(I2C1CONbits.SEN);
}

void i2c1_stop(void){
	while((I2C1CON & 0x1F) != 0);
	I2C1CONbits.PEN = 1;
	while(I2C1CONbits.PEN);
}

int i2c1_send(unsigned char value){
	I2C1TRN = value;
	while(I2C1STATbits.TRSTAT);
	return I2C1STATbits.ACKSTAT;
}

char i2c1_receive(char ack_bit){
	while((I2C1CON & 0x1F) != 0);
	I2C1CONbits.RCEN = 1;
	while(I2C1STATbits.RBF==0);
	if(ack_bit==0 || ack_bit==1){
		I2C1CONbits.ACKDT = ack_bit;
	}else{
		return 0;
	}
	I2C1CONbits.ACKEN = 1;
	while(I2C1CONbits.ACKEN);
	return I2C1RCV;
}
