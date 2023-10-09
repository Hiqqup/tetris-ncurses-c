#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#define DELAY 500000
int running = 1;

int x = 0;
int y = 0;
int ch;

void reprint()
{
    mvprintw(y, x, "o");
}
void handleInput()
{
    while (running) {
        ch = getch();
        if (ch == 32)
            y++;
        reprint();
    }
}
void* gameTick()
{
    while (running) {
        x++;
        reprint();
        refresh();
        usleep(DELAY);
    }
    return NULL;
}

int main()
{
    initscr();

    pthread_t gameTickThread;

    pthread_create(&gameTickThread, NULL, gameTick, NULL);

    handleInput();

    pthread_join(gameTickThread, NULL);

    return 0;
}
