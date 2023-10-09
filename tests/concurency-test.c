
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<ncurses.h>

#define DELAY 200000

int x = 0;
int y = 0;
int gameRunning =1;
int ch;

void* gameLoop( void * args){
	while(gameRunning){
		usleep(DELAY);
		x++;
	}
}
void* draw(void* args){
	initscr();
	noecho();
	timeout(DELAY/1000);
	while(gameRunning){
		ch = getch();
		if(ch == 32){
			y++;
		}
		mvprintw(x, y, "o");
	}

}
int main(){
	pthread_t inputThread;
	pthread_t drawThread;
	pthread_create(&inputThread, NULL, gameLoop, NULL);
	pthread_create(&drawThread, NULL, draw, NULL);

	while(gameRunning){
		sleep(1);
	}
	endwin();
	return 0;
}
