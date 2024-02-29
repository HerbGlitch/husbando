#include "ncurses.h"

#include <ncurses.h>
#include <stdlib.h>

struct ARC_NCurses {
    WINDOW *window;
};

void ARC_NCurses_Create(ARC_NCurses **ncurses){
    *ncurses = (ARC_NCurses *)malloc(sizeof(ARC_NCurses));

    
}

void ARC_NCurses_Destroy(ARC_NCurses *ncurses){
    free(ncurses);
}


