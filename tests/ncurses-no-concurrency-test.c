
#include<ncurses.h>
#include<unistd.h>
#include<pthread.h>

#define DELAY 300000

int ch;
int x=0;
int y=0;
void * getInput(void* args){
	while(1){
	ch = getch();
	}
}
int main(){
	initscr();
	timeout(DELAY/1000);
	noecho();

	pthread_t inputThread;
	pthread_create(&inputThread, NULL, getInput, NULL);

	while(1){
		if(ch == 32){
			y++;
		}
		x++;
		mvprintw(x,y,"o");
		usleep(DELAY);
	}
	endwin();
	return 0;
}
