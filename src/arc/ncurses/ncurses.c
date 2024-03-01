#include "ncurses.h"

#include <ncurses.h>
#include <stdlib.h>
#include <arc/std/errno.h>

uint8_t arc_ncurses_win_size = 0;

struct ARC_NCurses {
    WINDOW *window;
};

void ARC_NCurses_Create(ARC_NCurses **ncurses, ARC_Rect *windowBounds){
    if(arc_ncurses_win_size == ~(uint8_t)0){
        arc_errno = ARC_ERRNO_OVERFLOW;
        ARC_DEBUG_ERR("ARC_NCurses_Create(ncurses), max num of ARC_NCurses have been created, consider making arc_ncurses_win_size a uint32_t to increase the max");
        *ncurses = NULL;
        return;
    }

    //if this is the first ncurses, init ncurses
    if(arc_ncurses_win_size == 0){
        initscr();
        start_color();
        cbreak();
        keypad(stdscr, TRUE);
        refresh();
    }

    *ncurses = (ARC_NCurses *)malloc(sizeof(ARC_NCurses));

    ARC_Rect bounds = { 0, 0, COLS, LINES };
    if(windowBounds != NULL){
        bounds = *windowBounds;
    }

    (*ncurses)->window = newwin(bounds.h, bounds.w, bounds.y, bounds.x);

    wrefresh((*ncurses)->window);

    arc_ncurses_win_size++;
}

void ARC_NCurses_Destroy(ARC_NCurses *ncurses){
    arc_ncurses_win_size--;
    if(arc_ncurses_win_size == 0){
        endwin();
    }

    delwin(ncurses->window);
    free(ncurses);
}

void ARC_NCurses_SetBorder(ARC_NCurses *ncurses, uint32_t border){
    switch(border){
        case ARC_NCURSES_BORDER_NONE:
            wborder(ncurses->window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
            break;
        case ARC_NCURSES_BORDER_DEFAULT:
            box(ncurses->window, 0, 0);
            break;
    }

    wrefresh(ncurses->window);
}
