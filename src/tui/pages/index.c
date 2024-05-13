#include "index.h"

#include "tui/page.h"
#include <arc/console/buffer.h>
#include <arc/std/bool.h>

void HUSBANDO_TUIPage_CreateIndex(HUSBANDO_TUIPage **page){
    HUSBANDO_TUIPage_Create(page, HUSBANDO_TUIPage_MainIndex, HUSBANDO_TUIPage_PollIndex);
}

void HUSBANDO_TUIPage_DestroyIndex(HUSBANDO_TUIPage *page){
    HUSBANDO_TUIPage_Destroy(page);
}

void HUSBANDO_TUIPage_MainIndex(void *data){
}

void HUSBANDO_TUIPage_PollIndex(ARC_ConsoleBuffer *buffer, ARC_Bool *updateView, void *data){
    ARC_ConsoleBuffer_AddChar(buffer, 'A');
    *updateView = ARC_True;
}
