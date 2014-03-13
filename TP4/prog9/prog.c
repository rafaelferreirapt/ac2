void send2displays(char);
void delay(unsigned int);

int main(void){
	char c = 0;
	while(1){
		delay(200);
		send2displays(c);
		c++&0x00FF;
	}
	return 0;
}
