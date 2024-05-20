#include "page.h"

#include <arc/console/view.h>
#include <stdlib.h>

void HUSBANDO_TUIPage_Create(HUSBANDO_TUIPage **page, HUSBANDO_TUIPage_MainFn mainFn, HUSBANDO_TUIPage_PollFn pollFn, ARC_Rect bounds){
    *page = (HUSBANDO_TUIPage *)malloc(sizeof(HUSBANDO_TUIPage));

    ARC_ConsoleView_Create(&((*page)->view), bounds);

    (*page)->data = NULL;

    (*page)->mainFn = mainFn;
    (*page)->pollFn = pollFn;
}

void HUSBANDO_TUIPage_Destroy(HUSBANDO_TUIPage *page){
    free(page);
}
