/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
void send2displays(char);
void delay(unsigned int);

int main(void){
	char c = 0;
	int i;

	while(1){
		i = 0;
		do{
			delay(50);
			send2displays(c);
		}while(++i<4);
		c++&0x00FF;
	};
	return 0;
}
