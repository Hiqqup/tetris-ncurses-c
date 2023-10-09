#include<ncurses.h>

/*
  keys to note:
 
a: 97
d: 100
s: 115
w: 119
j: 106
k: 107
space: 32

  */
 
int main(){
	initscr();
	//noecho();
	int ch;
	while(1){
		ch = getch();
		printw("%i", ch);
		refresh();
	}
	endwin();
	return 0;
}
