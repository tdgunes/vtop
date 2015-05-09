#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "process.h"

#define HEADER_COLOR 1
#define SELECTED 2
#define NORMAL 3


struct v_process {
    int pid;
    char* user;
    float cpu;
    char* command;
};

int TERM_ROWS = 0, TERM_COLS = 0;
int CURSOR = 0, START_INDEX = 0, COUNT = 0;

void addline(const char* str);
void add_fline(const char *str, ...);
void draw(v_process** v_proc_list, size_t* proc_num);
void refresh(v_process** & v_proc_list, size_t* proc_num);

int main(int argc, char **argv) {



    WINDOW* window = initscr();
    noecho();
    signal (SIGWINCH, NULL);
    keypad(stdscr, TRUE);   // for KEY_UP, KEY_DOWN
    getmaxyx(stdscr, TERM_ROWS, TERM_COLS);


    start_color();
    init_pair(SELECTED, COLOR_BLACK, COLOR_CYAN);
    init_pair(NORMAL, COLOR_WHITE, COLOR_BLACK);
    init_pair(HEADER_COLOR, COLOR_BLACK, COLOR_GREEN);





    size_t proc_num = 0;
    v_process** v_proc_list = NULL;


    refresh(v_proc_list, &proc_num);



    addstr("\n");

    draw(v_proc_list, &proc_num);

    int ch;
    while ((ch = getch()) != EOF && ch != 'q') {


        if (ch == KEY_UP) {
            if (CURSOR == 0){
                if (START_INDEX > 0)
                    START_INDEX--;

            }
            else {
                CURSOR--;
            }
        }
        else if (ch == KEY_DOWN) {
            if ((START_INDEX+CURSOR) <= proc_num - 2) {
                if (CURSOR == TERM_ROWS - 2) {
                    START_INDEX++;
                }
                else {
                    CURSOR++;
                }
            }


        }
        else if (ch == KEY_RIGHT){
            v_process* p = v_proc_list[CURSOR];
            kill(p->pid, SIGKILL);
            refresh(v_proc_list, &proc_num);
        }
        draw(v_proc_list, &proc_num);
    }


    endwin();
    return 0;
}

void refresh(v_process** & v_proc_list, size_t* proc_num) {
    CURSOR = 0;
    if (v_proc_list != NULL) {
        for (int j = 0; j < *proc_num; ++j) {
            struct v_process* p=  v_proc_list[j];
            if (p != NULL)
                free(p);
        }
    }

    (*proc_num) = 0;
    kinfo_proc* proc_list = NULL;
    get_bsd_process_list(&proc_list, proc_num);

    v_proc_list = (v_process**) malloc(*proc_num * sizeof(struct v_process *));

    for (int i = 0; i < *proc_num; ++i) {
        struct kinfo_proc* process = &proc_list[i];
        struct passwd* user = getpwuid(process->kp_eproc.e_ucred.cr_uid);
        struct v_process* x = (v_process*) malloc(sizeof *x);
        x->command = process->kp_proc.p_comm;
        x->user = user->pw_name;
        x->pid = process->kp_proc.p_pid;
        v_proc_list[i] = x;
    }


}

void draw(v_process** v_proc_list, size_t* proc_num) {
    clear();

    attron(COLOR_PAIR(HEADER_COLOR));
    addline("   PID    USER COMMAND");
    attroff(COLOR_PAIR(HEADER_COLOR));

    if (v_proc_list!=NULL){
        COUNT = 0;

        for (int i = START_INDEX; i < *proc_num; ++i) {
            if (COUNT < TERM_ROWS){
                v_process* p = v_proc_list[i];

                if (CURSOR+START_INDEX == i) {
                    attron(COLOR_PAIR(SELECTED));
                    add_fline(" %5i %7s %s", p->pid,  p->user, p->command);
                    attroff(COLOR_PAIR(SELECTED));
                }
                else {
                    attron(COLOR_PAIR(NORMAL));
                    add_fline(" %5i %7s %s", p->pid,  p->user, p->command);
                    attroff(COLOR_PAIR(NORMAL));
                }

            }
            COUNT++;
        }
    }
    else {
        addline("v_proc_list is NULL\n");
    }
}

void add_fline(const char *str, ...) {
    char line[TERM_COLS];
    va_list args;
    va_start(args, str);
    vsnprintf(line, TERM_COLS-1, str, args);
    addline(line);
    va_end(args);
}

void addline(const char *str){
    char line[TERM_COLS];

    size_t empty_length = TERM_COLS-strlen(str);
    char empty[empty_length];
    for (int i = 0; i < empty_length; ++i) {
        empty[i] = ' ';
    }
    empty[empty_length-2] = '\n';
    empty[empty_length-1] = '\0';

    strcpy(line, str);
    strcat(line, empty);
    addstr(line);
}
