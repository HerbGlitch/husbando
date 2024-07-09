#include "search.h"

#include "arc/std/string.h"
#include "core/core.h"
#include "tui/base.h"
#include "tui/container.h"
#include "tui/page.h"
#include <stdlib.h>
#include <string.h>
#include <arc/console/view.h>
#include <arc/math/rectangle.h>
#include <arc/std/bool.h>

//create the data struct for the page
typedef struct HUSBANDO_TUIPageSearchData {
    HUSBANDO_TUIContainer *container;
    HUSBANDO_TUIBase *base;

    ARC_Vector *shows;
    ARC_Bool updateShows;
} HUSBANDO_TUIPageSearchData;

void HUSBANDO_TUIPage_CreateSearch(HUSBANDO_TUIPage **page, HUSBANDO_TUIContainer *container, ARC_Rect bounds){
    HUSBANDO_TUIPage_Create(page, HUSBANDO_TUIPage_MainSearch, HUSBANDO_TUIPage_PollSearch, bounds);
    ARC_ConsoleView_SetBorder((*page)->view, ARC_CONSOLE_VIEW_BORDER_DEFAULT);

    //create page index data
    HUSBANDO_TUIPageSearchData *searchData = (HUSBANDO_TUIPageSearchData *)malloc(sizeof(HUSBANDO_TUIPageSearchData));
    searchData->container = container;
    HUSBANDO_TUIBase_Create(&(searchData->base), container, (ARC_Rect){ bounds.x, bounds.y, bounds.w, 5 }, container->title);
    searchData->shows = NULL;
    searchData->updateShows = ARC_False;

    (*page)->data = (void *)searchData;

    uint64_t currentConsoleSearchStringLength = strlen(container->consoleSearchString->data);
    ARC_String *showNameString;
    ARC_String_CopySubstring(&showNameString, container->consoleSearchString, 0, currentConsoleSearchStringLength - 1);
    HUSBANDO_TUIPage_ExecuteSearch(*page, showNameString);
    ARC_String_Destroy(showNameString);
}

void HUSBANDO_TUIPage_DestroySearch(HUSBANDO_TUIPage *page){
    HUSBANDO_TUIPageSearchData *searchData = (HUSBANDO_TUIPageSearchData *)(page->data);

    if(searchData->shows != NULL){
        for(uint32_t i = 0; i < ARC_Vector_Size(searchData->shows); i++){
            HUSBANDO_CoreProviderShow *show = (HUSBANDO_CoreProviderShow *)ARC_Vector_Get(searchData->shows, i);
            ARC_String_Destroy(show->name);
            ARC_String_Destroy(show->language);
            ARC_String_Destroy(show->providerId);
            free(show);
        }

        ARC_Vector_Destroy(searchData->shows);
    }

    free(searchData);

    HUSBANDO_TUIPage_Destroy(page);
}

void HUSBANDO_TUIPage_MainSearch(ARC_ConsoleView *view, void *data){
    HUSBANDO_TUIPageSearchData *searchData = (HUSBANDO_TUIPageSearchData *)data;
    HUSBANDO_TUIBase_Main(searchData->base);

    //handle index page content here
    if(searchData->updateShows == ARC_True){
        for(uint32_t i = 0; i < ARC_Vector_Size(searchData->shows); i++){
            HUSBANDO_CoreProviderShow *show = (HUSBANDO_CoreProviderShow *)ARC_Vector_Get(searchData->shows, i);
            ARC_ConsoleView_RenderStringAt(view, show->name, (ARC_Point){ 2, 5 + i });
        }

        // searchData->updateShows = ARC_False;
    }
}

void HUSBANDO_TUIPage_PollSearch(ARC_ConsoleView *view, void *data){
    HUSBANDO_TUIPageSearchData *searchData = (HUSBANDO_TUIPageSearchData *)data;
    HUSBANDO_TUIBase_PollIndex(searchData->base);

    //handle index page poll content here
    if(searchData->updateShows == ARC_True){
        for(uint32_t i = 0; i < ARC_Vector_Size(searchData->shows); i++){
            HUSBANDO_CoreProviderShow *show = (HUSBANDO_CoreProviderShow *)ARC_Vector_Get(searchData->shows, i);
            ARC_ConsoleView_RenderStringAt(view, show->name, (ARC_Point){ 2, 5 + i });
        }

        // searchData->updateShows = ARC_False;
    }
}

void HUSBANDO_TUIPage_ExecuteSearch(HUSBANDO_TUIPage *page, ARC_String *searchString){
    HUSBANDO_TUIPageSearchData *searchData = (HUSBANDO_TUIPageSearchData *)(page->data);
    if(searchData->shows != NULL){
        for(uint32_t i = 0; i < ARC_Vector_Size(searchData->shows); i++){
            HUSBANDO_CoreProviderShow *show = (HUSBANDO_CoreProviderShow *)ARC_Vector_Get(searchData->shows, i);
            ARC_String_Destroy(show->name);
            ARC_String_Destroy(show->language);
            ARC_String_Destroy(show->providerId);
            free(show);
        }

        ARC_Vector_Destroy(searchData->shows);
    }

    ARC_String *tempLanguage;
    ARC_String_CreateWithStrlen(&tempLanguage, "dub");
    searchData->shows = husbando_core->provider->searchFn(husbando_core->provider, searchString, tempLanguage);

    searchData->updateShows = ARC_True;

    ARC_String_Destroy(tempLanguage);
}
