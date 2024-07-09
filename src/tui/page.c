#include "page.h"

#include "pages/index.h"
#include "pages/search.h"
#include <arc/console/view.h>
#include <arc/std/errno.h>
#include <stdlib.h>

void HUSBANDO_TUIPage_Create(HUSBANDO_TUIPage **page, HUSBANDO_TUIPage_MainFn mainFn, HUSBANDO_TUIPage_PollFn pollFn, ARC_Rect bounds){
    *page = (HUSBANDO_TUIPage *)malloc(sizeof(HUSBANDO_TUIPage));

    ARC_ConsoleView_Create(&((*page)->view), bounds);

    (*page)->data = NULL;

    (*page)->mainFn = mainFn;
    (*page)->pollFn = pollFn;
}

void HUSBANDO_TUIPage_CreateWithId(HUSBANDO_TUIPage **page, HUSBANDO_TUIPageId pageId, HUSBANDO_TUIContainer *container, ARC_Rect bounds){
    switch(pageId){
        case HUSBANDO_TUI_PAGE_ID_INDEX:
            HUSBANDO_TUIPage_CreateIndex(page, container, bounds);
            break;

        case HUSBANDO_TUI_PAGE_ID_SEARCH:
            HUSBANDO_TUIPage_CreateSearch(page, container, bounds);
            break;

        default:
            arc_errno = ARC_ERRNO_DATA;
            ARC_DEBUG_ERR("HUSBANDO_TUIPage_CreateWithId(page, pageId, container, bounds), could not create page with an id of none or unknown");
            break;
    }
}

void HUSBANDO_TUIPage_Destroy(HUSBANDO_TUIPage *page){
    if(page == NULL){
        return;
    }

    switch(page->pageId){
        case HUSBANDO_TUI_PAGE_ID_INDEX:
            page->pageId = HUSBANDO_TUI_PAGE_ID_NONE;
            HUSBANDO_TUIPage_DestroyIndex(page);
            break;

        case HUSBANDO_TUI_PAGE_ID_SEARCH:
            page->pageId = HUSBANDO_TUI_PAGE_ID_NONE;
            HUSBANDO_TUIPage_DestroySearch(page);
            break;

        default:
            ARC_ConsoleView_Destroy(page->view);
            free(page);
    }
}
