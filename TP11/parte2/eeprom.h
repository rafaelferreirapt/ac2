/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
// eeprom.h
#ifndef EEPROM_H
#define EEPROM_H

#define fPBCLK 20000000
#define RDSR   0x05
#define WRITE  0x02
#define READ   0x03
#define EEPROM_CLOCK 300000
#define WREN   0x06
#define WRDI   0x04

void spi2_init(void);
char eeprom_readStatus(void);
void eeprom_writeStatusCommand(char command);
void eeprom_writeData(int address, char value);
char eeprom_readData(int address);

#endif
