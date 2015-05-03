#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

int main() {
    WINDOW* mainWindow = initscr();

    if ( mainWindow== NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    mvaddstr(13,33, "Hello, World!");
    refresh();

    sleep(3);

    delwin(mainWindow);
    endwin();
    refresh();


    return EXIT_SUCCESS;
}