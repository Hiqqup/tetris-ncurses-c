#include<stdio.h>
#define LENGTH 40
#define HEIGHT 20

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main(){
	printf("\x1b[7m");
	for(int i = 0; i<HEIGHT; i++){

		for(int j = 0; j< LENGTH;j++){
			if(j == LENGTH/2 && i == HEIGHT/2)
			{
				printf(ANSI_COLOR_CYAN);
			}
			printf(" ");
		}
		printf("\n");
	}
	printf("\x1b[0m");


	return 0;
}
