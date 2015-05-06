#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

//#define MAXLEN 128
//
//typedef struct Line {
//    char contents[MAXLEN];
//    struct Line *prev, *next;
//} Line;
//
//Line *head, *curr;
//
//int nr_lines;
//int curr_line;
//

//
//void load(const char *filename);
//void draw(Line *l);





struct v_process {
    int pid;
    char* user;
    float cpu;
    char* command;
};


int TERM_ROWS, TERM_COLS;

void addline(const char* str);

int main(int argc, char **argv) {


    kinfo_proc* proc_list = NULL;
    size_t proc_num = 0;
    get_bsd_process_list(&proc_list, &proc_num);
    v_process* v_proc_list[proc_num];




    WINDOW* window = initscr();
    noecho();
    signal (SIGWINCH, NULL);
    keypad(stdscr, TRUE);   // for KEY_UP, KEY_DOWN
    getmaxyx(stdscr, TERM_ROWS, TERM_COLS);



    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    attron(COLOR_PAIR(1));

    addline(" PID USER CPU% COMMAND");

    attroff(COLOR_PAIR(1));


    for (int i = 0; i < proc_num; ++i) {
        struct kinfo_proc* process = &proc_list[i];
        struct passwd* user = getpwuid(process->kp_eproc.e_ucred.cr_uid);
        wprintw(window, " %i %s %s\n", process->kp_proc.p_pid,  user->pw_name ,process->kp_proc.p_comm);
    }



    addstr("\n");


//    addline(" CPU: 12.4%\n");
//    wprintw(window, "x%i y:%i\n",TERM_ROWS, TERM_COLS);
//    addstr(" Mem: 1204/8123M (12.4%)\n");
//    addstr(" Swap 10.2%\n");
//    addstr(" Tasks: \n");
//    addstr(" Uptime: 1 day, 00:51:33\n");
//    addstr("\n");



//    for (int i = 0; i < TERM_ROWS; ++i) {
//        wprintw(window, "%i\n",i);
//    }

    int ch;
    while ((ch = getch()) != EOF && ch != 'q') {


//        if (ch == KEY_UP && curr->prev != NULL) {
//            curr_line--;
//            curr = curr->prev;
//        }
//        else if (ch == KEY_DOWN && curr->next != NULL
//                 && curr_line + TERM_ROWS < nr_lines) {
//            curr_line++;
//            curr = curr->next;
//        }
//        draw(curr);
    }

//    load(argv[1]);
//
//    curr = head;
//    curr_line = 0;
//    draw(curr);
//
//    int ch;
//    while ((ch = getch()) != EOF && ch != 'q')
//    {
//        if (ch == KEY_UP && curr->prev != NULL) {
//            curr_line--;
//            curr = curr->prev;
//        }
//        else if (ch == KEY_DOWN && curr->next != NULL
//                 && curr_line + TERM_ROWS < nr_lines) {
//            curr_line++;
//            curr = curr->next;
//        }
//        draw(curr);
//    }

    endwin();
    return 0;
}

void addline(const char *str){
    char line[TERM_COLS];

    size_t empty_length = TERM_COLS-strlen(str);
    char empty[empty_length];
    for (int i = 0; i < empty_length; ++i) {
        empty[i] = ' ';
    }
    empty[empty_length-1] = '\0';

    strcpy(line, str);
    strcat(line, empty);
    addstr(line);
}

//void load(const char *filename) {
//
//
//
//
//    FILE *fp = fopen(filename, "r");
//    Line *lp;
//
//    head = (Line *)malloc(sizeof(Line));
//    head->prev = head->next = NULL;
//    curr = head;
//
//    while (fgets(curr->contents, MAXLEN, fp))
//    {
//        lp = (Line *)malloc(sizeof(Line));
//        lp->prev = curr;
//        curr->next = lp;
//        curr = lp;
//        nr_lines++;
//    }
//    curr->next = NULL;
//}
//
//void draw(Line *l)
//{
//    int i;
//    clear();
//    for (i = 0; i < TERM_ROWS && l != NULL; i++, l = l->next)
//        addstr(l->contents);
//}