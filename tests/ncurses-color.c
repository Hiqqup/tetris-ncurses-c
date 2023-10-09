
#include <curses.h>
#include <ncurses.h>

int main()
{
    initscr();
    start_color();
    curs_set(0);
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    attron(COLOR_PAIR(1));
    mvprintw(10, 16, "  ");
    mvprintw(10, 18, "  ");
    mvprintw(11, 18, "  ");
    mvprintw(10, 20, "  ");
    refresh();
    attroff(COLOR_PAIR(1));
    getch();
    endwin();
    return 0;
}
