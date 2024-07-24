#include "search.h"

#include "core/controls.h"
#include "core/core.h"
#include "tui/base.h"
#include "tui/container.h"
#include "tui/page.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arc/console/key.h>
#include <arc/console/view.h>
#include <arc/math/rectangle.h>
#include <arc/std/bool.h>
#include <arc/std/queue.h>
#include <arc/std/string.h>

//create the data struct for the page
typedef struct HUSBANDO_TUIPageSearchData {
    HUSBANDO_TUIContainer *container;
    HUSBANDO_TUIBase *base;

    ARC_Vector *shows;
    ARC_Bool updateShows;

    uint32_t selectedIndex;
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
    searchData->selectedIndex = 0;

    (*page)->data = (void *)searchData;

    uint64_t currentConsoleSearchStringLength = strlen(container->consoleSearchString->data);
    ARC_String *showNameString;
    ARC_String_CopySubstring(&showNameString, container->consoleSearchString, 0, currentConsoleSearchStringLength - 1);
    HUSBANDO_TUIPage_ExecuteSearch(*page, showNameString);
    ARC_String_Destroy(showNameString);

    ARC_ConsoleView_RenderCharAt((*page)->view, '#', (ARC_Point){ 5, 5 });
    ARC_ConsoleView_RenderCStringWithStrlenAt((*page)->view, "Title (english)", (ARC_Point){ 10, 5 });
    ARC_ConsoleView_RenderCharAt((*page)->view, '|', (ARC_Point){ 4, 6 + searchData->selectedIndex });
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

    //handle key inputs
    while(ARC_Queue_Size(searchData->base->container->consoleKeyQueue) != 0){
        ARC_ConsoleKey *key = (ARC_ConsoleKey *)ARC_Queue_Pop(searchData->base->container->consoleKeyQueue);

        //up
        if(ARC_ConsoleKey_EqualsPointer(key, ARC_CONSOLE_KEY_K)){
            if(searchData->selectedIndex > 0){
                ARC_ConsoleView_RenderCharAt(view, ' ', (ARC_Point){ 4, 6 + searchData->selectedIndex });
                searchData->selectedIndex--;
                ARC_ConsoleView_RenderCharAt(view, '|', (ARC_Point){ 4, 6 + searchData->selectedIndex });
            }
            ARC_ConsoleKey_Destroy(key);
            continue;
        }

        //down
        if(ARC_ConsoleKey_EqualsPointer(key, ARC_CONSOLE_KEY_J)){
            if(searchData->selectedIndex < ARC_Vector_Size(searchData->shows) - 1){
                ARC_ConsoleView_RenderCharAt(view, ' ', (ARC_Point){ 4, 6 + searchData->selectedIndex });
                searchData->selectedIndex++;
                ARC_ConsoleView_RenderCharAt(view, '|', (ARC_Point){ 4, 6 + searchData->selectedIndex });
            }
            ARC_ConsoleKey_Destroy(key);
            continue;
        }

        if(ARC_ConsoleKey_EqualsPointer(key, ARC_CONSOLE_KEY_ENTER)){
            if(searchData->selectedIndex >= ARC_Vector_Size(searchData->shows)){
                //TODO: throw error

                //TEMP: set to 0 on error
                searchData->selectedIndex = 0;
            }

            HUSBANDO_CoreProviderShow *show = (HUSBANDO_CoreProviderShow *)ARC_Vector_Get(searchData->shows, searchData->selectedIndex);
            HUSBANDO_CoreProviderEpisode *episode = husbando_core->provider->getEpisodeFn(husbando_core->provider, show, 1);
            HUSBANDO_Core_ControlsInit(husbando_core, episode->url, ARC_True);
            continue;
        }

        ARC_ConsoleKey_Destroy(key);
    }

    //handle index page content here
    if(searchData->updateShows == ARC_True){
        for(uint32_t index = 0; index < ARC_Vector_Size(searchData->shows); index++){
            HUSBANDO_CoreProviderShow *show = (HUSBANDO_CoreProviderShow *)ARC_Vector_Get(searchData->shows, index);

            char indexCString[11];
            sprintf(indexCString, "%u", index);

            ARC_ConsoleView_RenderCStringWithStrlenAt(view, indexCString, (ARC_Point){ 5, 6 + index });
            ARC_ConsoleView_RenderStringAt(view, show->name, (ARC_Point){ 10, 6 + index });
        }

        searchData->updateShows = ARC_False;
    }
}

void HUSBANDO_TUIPage_PollSearch(ARC_ConsoleView *view, void *data){
    HUSBANDO_TUIPageSearchData *searchData = (HUSBANDO_TUIPageSearchData *)data;
    HUSBANDO_TUIBase_PollIndex(searchData->base);

    //handle index page poll content here
    if(searchData->updateShows == ARC_True){
        for(uint32_t index = 0; index < ARC_Vector_Size(searchData->shows); index++){
            HUSBANDO_CoreProviderShow *show = (HUSBANDO_CoreProviderShow *)ARC_Vector_Get(searchData->shows, index);

            char indexCString[11];
            sprintf(indexCString, "%u", index);

            ARC_ConsoleView_RenderCStringWithStrlenAt(view, indexCString, (ARC_Point){ 5, 6 + index });
            ARC_ConsoleView_RenderStringAt(view, show->name, (ARC_Point){ 10, 6 + index });
        }

        searchData->updateShows = ARC_False;
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
