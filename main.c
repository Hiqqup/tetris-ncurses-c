#include <curses.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define BOARD_LENGTH 10
#define BOARD_HEIGHT 20

#define GROUP_GAMEOVERLINE 10
#define GROUP_WHITE 1
#define GROUP_BLACK 2
#define GROUP_YELLOW 3
#define GROUP_GREEN 4
#define GROUP_BLUE 5
#define GROUP_CYAN 6
#define GROUP_MAGENTA 7
#define GROUP_RED 8

int offsetX = 5;
int offsetY = 2;

int tickSpeed = 400000;
int ch;
int gameRunning = 1;

int currentOffsets[4][2];
short currentColor;
int anchorY;
int anchorX;

int checked[4];

short frozenPieces[BOARD_LENGTH][BOARD_HEIGHT];

// offsets
const int PIECE_OFFSETS[7][4][2] = {
    // l piece
    { { 0, 0 },
        { -1, 0 },
        { 1, 0 },
        { 1, 1 } },
    // s piece
    { { 0, 0 },
        { -1, 0 },
        { 0, 1 },
        { 1, 1 } },
    // j piece
    { { 0, 0 },
        { -1, 0 },
        { 1, 0 },
        { -1, 1 } },
    // l piece
    { { 0, 0 },
        { -1, 0 },
        { 1, 0 },
        { 2, 0 } },
    // t piece
    { { 0, 0 },
        { -1, 0 },
        { 1, 0 },
        { 0, 1 } },
    // z piece
    { { 0, 0 },
        { -1, 1 },
        { 1, 0 },
        { 0, 1 } },
    // o piecie
    { { 0, 0 },
        { 0, 1 },
        { 1, 0 },
        { 1, 1 } }
};

void ncursesSetup()
{
    WINDOW* win = initscr();
    noecho();
    curs_set(0);
    int maxX;
    int maxY;
    getmaxyx(win, maxY, maxX);

    if (maxY < BOARD_HEIGHT + 3 || maxX < (BOARD_LENGTH + 2) * 2) {
        endwin();
        printf("Zoom you terminal out a little bit \n");
        exit(1);
    }
    offsetX = (maxX - 20) / 4;
    offsetY = (maxY - 18) / 2;
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_GREEN);
    init_pair(5, COLOR_BLACK, COLOR_BLUE);
    init_pair(6, COLOR_BLACK, COLOR_CYAN);
    init_pair(7, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(8, COLOR_BLACK, COLOR_RED);
    init_pair(9, COLOR_BLACK, COLOR_YELLOW);
    init_pair(10, COLOR_WHITE, COLOR_BLACK);
    // timeout(tickSpeed / 1000);
}
void printPix(int x, int y, short color)
{
    attron(COLOR_PAIR(color));
    mvprintw(offsetY + x, offsetX * 2 + y * 2, "  ");
    attroff(COLOR_PAIR(color));
}
void setCurrent()
{
    int index = rand() % 7;
    anchorX = BOARD_LENGTH / 2;
    anchorY = 0;
    for (int i = 0; i < 4; i++) {
        currentOffsets[i][0] = PIECE_OFFSETS[index][i][0];
        currentOffsets[i][1] = PIECE_OFFSETS[index][i][1];
    }
    currentColor = index + 3;
}
void printCurrent()
{
    for (int i = 0; i < 4; i++) {
        printPix(anchorY + currentOffsets[i][0], anchorX + currentOffsets[i][1], currentColor);
    }
}
void unprintCurrent()
{
    for (int i = 0; i < 4; i++) {
        printPix(anchorY + currentOffsets[i][0], anchorX + currentOffsets[i][1], GROUP_BLACK);
    }
}
void printBoard()
{
    for (int i = -2; i < BOARD_HEIGHT; i++) {
        printPix(i, -1, GROUP_WHITE);
    }
    for (int i = -2; i < BOARD_HEIGHT; i++) {
        printPix(i, BOARD_LENGTH, GROUP_WHITE);
    }
    for (int i = -1; i < BOARD_LENGTH + 1; i++) {
        printPix(BOARD_HEIGHT, i, GROUP_WHITE);
    }
    for (int i = 0; i < offsetX * 2 - 2; i++) {
        mvaddch(offsetY - 1, i, ACS_HLINE);
    }
    for (int i = (offsetX + BOARD_LENGTH + 2) * 2; i < (offsetX + BOARD_LENGTH + 2) * 2 + offsetX * 2; i++) {
        mvaddch(offsetY - 1, i, ACS_HLINE);
    }
    refresh();
}
int checkColision()
{
    for (int i = 0; i < 4; i++) {
        int y = anchorY + currentOffsets[i][0];
        int x = anchorX + currentOffsets[i][1];
        if (x < 0 || y > BOARD_HEIGHT - 1 || x >= BOARD_LENGTH || frozenPieces[x][y] != 0) {
            return 1;
        }
    }
    return 0;
}
int checkCompletedRow(int y)
{
    for (int x = 0; x < BOARD_LENGTH; x++) {
        // mvprintw(y, x, "%i", frozenPieces[x][y]);
        if (frozenPieces[x][y] == 0) {
            return 0;
        }
    }
    return 1;
}
void clearRow(int startY)
{
    for (int y = startY; y > 0; y--) {
        for (int x = 0; x < BOARD_LENGTH; x++) {
            frozenPieces[x][y] = frozenPieces[x][y - 1];
        }
    }
}
void reprintFrozenPieces()
{
    clear();
    printBoard();

    for (int x = 0; x < BOARD_LENGTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if (frozenPieces[x][y])
                printPix(y, x, frozenPieces[x][y]);
        }
    }

    refresh();
}
/*
void debugFrozenPieces()
{
    for (int x = 0; x < BOARD_LENGTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            mvprintw(y, x * 2, "%i", frozenPieces[x][y]);
        }
    }
}
*/
void freezePiece()
{
    anchorY--;
    printCurrent();
    for (int i = 0; i < 4; i++) {
        int y = anchorY + currentOffsets[i][0];
        int x = anchorX + currentOffsets[i][1];
        frozenPieces[x][y] = currentColor;
        checked[i] = y;
    }
    for (int i = 0; i < 4; i++) {
        if (checkCompletedRow(checked[i])) {
            clearRow(checked[i]);
            reprintFrozenPieces();
        }
    }
    setCurrent();
}

void* gameTick(void* args)
{
    while (gameRunning) {
        unprintCurrent();
        anchorY++;
        if (checkColision()) {
            freezePiece();
        }
        printCurrent();
        // debugFrozenPieces();
        refresh();
        usleep(tickSpeed);
    }
    return NULL;
}

void rotate(int direction)
{
    for (int i = 0; i < 4; i++) {
        int buf = currentOffsets[i][1];
        currentOffsets[i][1] = -currentOffsets[i][0] * direction;
        currentOffsets[i][0] = buf * direction;
    }
}

void handleInput()
{
    while (gameRunning) {
        ch = getch();
        unprintCurrent();
        switch (ch) {

        case 97: /* a*/
            anchorX--;
            if (checkColision())
                anchorX++;
            break;
        case 100: /* d*/
            anchorX++;
            if (checkColision())
                anchorX--;
            break;
        case 115: /* s*/
            anchorY++;
            if (checkColision()) {
                freezePiece();
            }
            break;
        case 106: /* j*/
            rotate(1);
            if (checkColision())
                rotate(-1);
            break;
        case 107: /* k*/
            rotate(-1);
            if (checkColision())
                rotate(1);

            break;
        case 32: /* space */
            // quickdrop
            break;
        }
        printCurrent();
    }
}
int main()
{
    srand(time(NULL));
    ncursesSetup();

    printBoard();
    setCurrent();
    pthread_t tickThread;
    pthread_create(&tickThread, NULL, gameTick, NULL);
    handleInput();
    pthread_join(tickThread, NULL);
    endwin();
    return 0;
}
