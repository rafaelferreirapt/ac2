/*
Aulas resolvidas por Rafael Ferreira e Rodrigo Cunha.
https://github.com/gipmon/ac2
*/
void delay(unsigned int);
void send2display10(int, int);
int read7segments();

int main(void){
	int display = 0;
	while(1){
		int read = read7segments();
		send2display10(read, display);
		display = !display;
	}
}
