
#include<stdio.h>

#define	RED     "\x1b[41m  "
#define	GREEN   "\x1b[42m  "
#define	YELLOW  "\x1b[43m  "
#define	BLUE    "\x1b[44m  "
#define	MAGENTA "\x1b[45m  "
#define	CYAN    "\x1b[46m  "
#define	WHITE   "\x1b[47m  "
#define BLACK   "\x1b[40m  "
#define ORANGE "\x1b[7m\x1b[38;5;208m  "
#define DARKGRAY "\x1b[7m\x1b[38;5;238m  "
#define GRAY "\x1b[7m\x1b[38;5;247m  "
#define RESET_COLOR "\x1b[0m"
int main(){
	printf(YELLOW  RESET_COLOR "\n");
	printf(    ORANGE RESET_COLOR "\n");
	printf(RED  RESET_COLOR "\n");
	printf(MAGENTA  RESET_COLOR "\n");
	printf(BLUE RESET_COLOR "\n");
	printf(CYAN  RESET_COLOR "\n");
	printf(GREEN  RESET_COLOR "\n");
	printf(WHITE  RESET_COLOR "\n");
	printf(GRAY  RESET_COLOR "\n");
	printf(DARKGRAY  RESET_COLOR "\n");
	printf(BLACK  RESET_COLOR "\n");
	return 0;
}
