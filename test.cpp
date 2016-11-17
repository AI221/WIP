#include <ncurses.h>

int main(int argc, char *argv[])
{   initscr();          /* Start curses mode        */
        if(has_colors() == FALSE)
                {   endwin();
                            printf("Your terminal does not support color\n");
                                    //exit(1);
                                        }
            start_color();          /* Start color          */
    init_pair(1,COLOR_WHITE,COLOR_GREEN);

    init_pair(2,COLOR_WHITE,COLOR_WHITE);
                    attron(COLOR_PAIR(1));
                       mvprintw( 0, 0, "Viola !!! In color ...");
                            //attroff(COLOR_PAIR(1));
                            //attroff(COLOR_PAIR(1));
                            attron(COLOR_PAIR(2));
                            mvprintw(1,0,"hi...");
                            //attroff(COLOR_PAIR(1));
                                    getch();
                                        endwin();
}
