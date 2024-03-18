#include "ncurses.h"

#include "element.h"
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <arc/std/errno.h>
#include <arc/std/vector.h>
#include <arc/std/string.h>

uint8_t arc_ncurses_win_size = 0;

struct ARC_NCurses {
    WINDOW *window;

    ARC_Vector *elements;
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
        //start_color();
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

    ARC_Vector_Create(&(*ncurses)->elements);

    wrefresh((*ncurses)->window);

    arc_ncurses_win_size++;
}

void ARC_NCurses_Destroy(ARC_NCurses *ncurses){
    arc_ncurses_win_size--;

    ARC_Vector_Destroy(ncurses->elements);

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

void ARC_NCurses_SetAttribute(ARC_NCurses *ncurses, uint32_t attribute){
    switch(attribute){
        case ARC_NCURSES_ATTRIBUTE_NONE:
            wattroff(ncurses->window, A_REVERSE);
            break;
        case ARC_NCURSES_ATTRIBUTE_REVERSE:
            wattron(ncurses->window, A_REVERSE);
            break;
    }
}

void ARC_NCurses_AddElement(ARC_NCurses *ncurses, ARC_NCursesElement *element){
    ARC_Vector_Add(ncurses->elements, (void *)element);
}

ARC_NCursesElement *ARC_NCurses_GetElement(ARC_NCurses *ncurses, uint32_t index){
    return (ARC_NCursesElement *)ARC_Vector_Get(ncurses->elements, &index);
}

void ARC_NCurses_RemoveElement(ARC_NCurses *ncurses, uint32_t index){
    ARC_Vector_RemoveIndex(ncurses->elements, &index);
}

void ARC_NCurses_RenderStringAt(ARC_NCurses *ncurses, ARC_String *text, ARC_Point pos){
    mvwprintw(ncurses->window, pos.y, pos.x, "%s", text->data);
    wrefresh(ncurses->window);
}

void ARC_NCurses_RenderElements(ARC_NCurses *ncurses){
    for(uint32_t i = 0; i < *ARC_Vector_Size(ncurses->elements); i++){
        ARC_NCursesElement *element = (ARC_NCursesElement *)ARC_Vector_Get(ncurses->elements, &i);
        element->renderFn(ncurses, element);
        wrefresh(ncurses->window);
    }
}
