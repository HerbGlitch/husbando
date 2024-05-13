#include "page.h"

#include <stdlib.h>
#include <arc/console/buffer.h>

void HUSBANDO_TUIPage_Create(HUSBANDO_TUIPage **page, HUSBANDO_TUIPage_MainFn mainFn, HUSBANDO_TUIPage_PollFn pollFn){
    *page = (HUSBANDO_TUIPage *)malloc(sizeof(HUSBANDO_TUIPage));

    (*page)->data = NULL;

    (*page)->mainFn = mainFn;
    (*page)->pollFn = pollFn;
}

void HUSBANDO_TUIPage_Destroy(HUSBANDO_TUIPage *page){
    free(page);
}
