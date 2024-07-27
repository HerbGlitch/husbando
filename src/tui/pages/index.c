#include "index.h"

#include "tui/base.h"
#include "tui/container.h"
#include "tui/page.h"
#include <stdlib.h>
#include <arc/console/view.h>
#include <arc/math/rectangle.h>
#include <arc/std/bool.h>

//create the data struct for the page
typedef struct HUSBANDO_TUIPageIndexData {
    HUSBANDO_TUIContainer *container;
    HUSBANDO_TUIBase *base;
} HUSBANDO_TUIPageIndexData;

void HUSBANDO_TUIPage_CreateIndex(HUSBANDO_TUIPage **page, HUSBANDO_TUIContainer *container, ARC_Rect bounds){
    HUSBANDO_TUIPage_Create(page, HUSBANDO_TUIPage_MainIndex, HUSBANDO_TUIPage_PollIndex, bounds);
    ARC_ConsoleView_SetBorder((*page)->view, ARC_CONSOLE_VIEW_BORDER_DEFAULT);

    //create page index data
    HUSBANDO_TUIPageIndexData *indexData = (HUSBANDO_TUIPageIndexData *)malloc(sizeof(HUSBANDO_TUIPageIndexData));
    indexData->container = container;
    HUSBANDO_TUIBase_Create(&(indexData->base), container, (ARC_Rect){ bounds.x, bounds.y, bounds.w, 5 }, container->title);
    (*page)->data = (void *)indexData;

    //outline the page
    ARC_ConsoleView_RenderCharAt((*page)->view, '#', (ARC_Point){ 5, 5 });
    ARC_ConsoleView_RenderCStringWithStrlenAt((*page)->view, "Title (english)", (ARC_Point){ 10, 5 });
    ARC_ConsoleView_RenderCStringWithStrlenAt((*page)->view, "Episode", (ARC_Point){ 30, 5 });
    ARC_ConsoleView_RenderCStringWithStrlenAt((*page)->view, "Language", (ARC_Point){ 50, 5 });
    ARC_ConsoleView_RenderCStringWithStrlenAt((*page)->view, "Last Watched", (ARC_Point){ 70, 5 });
    ARC_ConsoleView_RenderCStringWithStrlenAt((*page)->view, "Started", (ARC_Point){ 90, 5 });
    ARC_ConsoleView_RenderCStringWithStrlenAt((*page)->view, "# Rewatches", (ARC_Point){ 110, 5 });
    ARC_ConsoleView_RenderCStringWithStrlenAt((*page)->view, "Rating", (ARC_Point){ 130, 5 });
    ARC_ConsoleView_RenderCStringWithStrlenAt((*page)->view, "Local", (ARC_Point){ 150, 5 });
    ARC_ConsoleView_RenderCStringWithStrlenAt((*page)->view, "Tags", (ARC_Point){ 170, 5 });
}

void HUSBANDO_TUIPage_DestroyIndex(HUSBANDO_TUIPage *page){
    HUSBANDO_TUIPageIndexData *indexData = (HUSBANDO_TUIPageIndexData *)(page->data);
    free(indexData);

    HUSBANDO_TUIPage_Destroy(page);
}

void HUSBANDO_TUIPage_MainIndex(ARC_ConsoleView *view, void *data){
    HUSBANDO_TUIPageIndexData *indexData = (HUSBANDO_TUIPageIndexData *)data;
    HUSBANDO_TUIBase_Main(indexData->base);

    //handle index page content here
}

void HUSBANDO_TUIPage_PollIndex(ARC_ConsoleView *view, void *data){
    HUSBANDO_TUIPageIndexData *indexData = (HUSBANDO_TUIPageIndexData *)data;
    HUSBANDO_TUIBase_PollIndex(indexData->base);

    //handle index page poll content here
}
