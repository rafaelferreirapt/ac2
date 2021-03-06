/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
void send2displays(char);
void delay(unsigned int);
unsigned char toBcd(unsigned char);

int main(void){
	unsigned char c = 0, tmp;
	int i;

	while(1){
		i = 0;
		do{
			delay(10);
			tmp = toBcd(c);
			send2displays(tmp);
		}while(++i<4);
		if(c==0x3B){
			c = 0;
		}else{
			c++;
		}
	};
	return 0;
}
