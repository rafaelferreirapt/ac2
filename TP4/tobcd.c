/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
unsigned char toBcd(unsigned char value){
	return ((value/10)<<4) + (value % 10);
}
