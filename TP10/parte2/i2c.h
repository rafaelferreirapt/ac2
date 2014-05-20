// i2c.h
#ifndef I2C_H
#define I2C_H

#define  I2C_READ		1
#define  I2C_WRITE		0
#define  I2C_ACK		0
#define  I2C_NACK		1

#define  ADDR_WR		((SENS_ADDRESS << 1) | I2C_WRITE)
#define  ADDR_RD		((SENS_ADDRESS << 1) | I2C_READ)
#define  SENS_ADDRESS	0x4D 	//device dependent
#define  TC74_CLK_FREQ	100000 	//100kHZ
#define  RTR            0 		//Read temperature command

void delay(unsigned int);
void i2c1_init(unsigned int clock_freq);
void i2c1_start(void);
void i2c1_stop(void);
int  i2c1_send(unsigned char value);
char i2c1_receive(char ack_bit);

#endif
