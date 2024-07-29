#include "container.h"

#include "page.h"
#include "core/controls.h"
#include "core/core.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arc/console/key.h>
#include <arc/console/view.h>
#include <arc/math/point.h>
#include <arc/math/rectangle.h>
#include <arc/std/bool.h>
#include <arc/std/errno.h>
#include <arc/std/queue.h>
#include <arc/std/string.h>

void HUSBANDO_TUIContainer_Create(HUSBANDO_TUIContainer **container, char *title, HUSBANDO_TUIPage *page, uint32_t pollTime){
    *container = (HUSBANDO_TUIContainer *)malloc(sizeof(HUSBANDO_TUIContainer));

    ARC_ConsoleView_Create(&((*container)->view), (ARC_Rect){ 0, 0, 0, 0 });
    ARC_Queue_Create(&((*container)->consoleKeyQueue));

    //todo change this to 128
    ARC_String_CreateEmpty(&((*container)->consoleSearchString), 36);

    (*container)->title = title;

    (*container)->page = page;
    (*container)->nextPageId = HUSBANDO_TUI_PAGE_ID_NONE;

    (*container)->inputMode = NORMAL;

    (*container)->cursor = (ARC_Point){ 0, 0 };
    (*container)->visibleCursor = ARC_False;

    ARC_ConsoleView_SetCursorVisibility((*container)->view, ARC_CONSOLE_VIEW_CURSOR_HIDDEN);

    (*container)->running = ARC_False;

    (*container)->pollTime = pollTime;

    //init the mutex for later
    if(pthread_mutex_init(&((*container)->bufferMutex), NULL) != 0){
        //the mutex could not be created so throw error
        arc_errno = ARC_ERRNO_INIT;
        ARC_DEBUG_ERR("HUSBANDO_TUIContainer_Create(container, title, page, pollTime), could not init buffer mutex");

        //the container cannot run without the mutex, so destroy the container and set the param value to NULL
        HUSBANDO_TUIContainer_ClearConsoleKeyQueue(*container);

        ARC_Queue_Destroy((*container)->consoleKeyQueue);
        ARC_ConsoleView_Destroy((*container)->view);

        free(*container);
        *container = NULL;
    }
}

void HUSBANDO_TUIContainer_Destory(HUSBANDO_TUIContainer *container){
    HUSBANDO_TUIContainer_ClearConsoleKeyQueue(container);

    ARC_Queue_Destroy(container->consoleKeyQueue);
    ARC_ConsoleView_Destroy(container->view);

    free(container);
}

//private type for polling
typedef struct HUSBANDO_TUIContainerPollParams {
    HUSBANDO_TUIContainer *container;
} HUSBANDO_TUIContainerPollParams;

//private function that runs the polling
void *HUSBANDO_TUIContainer_RunPoll(void *data){
    HUSBANDO_TUIContainerPollParams *pollParams = (HUSBANDO_TUIContainerPollParams *)data;

    pthread_mutex_lock(&(pollParams->container->bufferMutex));
    ARC_Bool threadRunning = pollParams->container->running;
    struct timespec sleepTime = { pollParams->container->pollTime / 1000, (pollParams->container->pollTime % 1000) * 1000000 };
    pthread_mutex_unlock(&(pollParams->container->bufferMutex));

    while(threadRunning){
        pthread_mutex_lock(&(pollParams->container->bufferMutex));
        HUSBANDO_TUIPage *page = pollParams->container->page;
        if(page != NULL){
            page->pollFn(page->view, page->data);
        }
        pthread_mutex_unlock(&(pollParams->container->bufferMutex));

        nanosleep(&sleepTime, &sleepTime);

        pthread_mutex_lock(&(pollParams->container->bufferMutex));
        threadRunning = pollParams->container->running;
        pthread_mutex_unlock(&(pollParams->container->bufferMutex));
    }

    return NULL;
}

void HUSBANDO_TUIContainer_RunPage(HUSBANDO_TUIContainer *container){
    if(container->page == NULL){
        arc_errno = ARC_ERRNO_NULL;
        ARC_DEBUG_ERR("HUSBANDO_TUIContainer_RunPage(container), page was NULL");
        return;
    }

    container->running = ARC_True;

    HUSBANDO_TUIContainerPollParams pollParams = {
        container
    };

    //create and check the poll thread
    int32_t pollThreadReturnValue = pthread_create(&(container->pollThread), NULL, HUSBANDO_TUIContainer_RunPoll, (void *)&pollParams);
    if(pollThreadReturnValue != 0){
        arc_errno = ARC_ERRNO_INIT;
        ARC_DEBUG_ERR("HUSBANDO_TUIContainer_RunPage(container), could not init poll pthread");
        return;
    }

    //update once before running
    pthread_mutex_lock(&(container->bufferMutex));
    container->page->mainFn(container->page->view, container->page->data);
    pthread_mutex_unlock(&(container->bufferMutex));

    //main thread, handle key inputs and exiting
    while(container->running){
        if(container->nextPageId != HUSBANDO_TUI_PAGE_ID_NONE){
            pthread_mutex_lock(&(container->bufferMutex));
            ARC_Rect bounds = ARC_ConsoleView_GetBounds(container->page->view);
            HUSBANDO_TUIPage_Destroy(container->page);

            HUSBANDO_TUIPage_CreateWithId(&(container->page), container->nextPageId, container, bounds);

            //update once before running
            container->page->mainFn(container->page->view, container->page->data);
            pthread_mutex_unlock(&(container->bufferMutex));

            container->nextPageId = HUSBANDO_TUI_PAGE_ID_NONE;
        }

        //handle input modes
        pthread_mutex_lock(&(container->bufferMutex));
        switch(container->inputMode){
            case NORMAL:
                HUSBANDO_TUIContainer_HandleNormalInput(container);
                break;

            case SEARCH:
                HUSBANDO_TUIContainer_HandleSearchInput(container);
                break;
        }
        pthread_mutex_unlock(&(container->bufferMutex));

        //update the page with the new input
        pthread_mutex_lock(&(container->bufferMutex));
        container->page->mainFn(container->page->view, container->page->data);
        pthread_mutex_unlock(&(container->bufferMutex));
    }

    //join the threads on end
    pthread_join(container->pollThread, NULL);
}

void HUSBANDO_TUIContainer_ClearConsoleKeyQueue(HUSBANDO_TUIContainer *container){
    while(ARC_Queue_Size(container->consoleKeyQueue) != 0){
        ARC_ConsoleKey *key = (ARC_ConsoleKey *)ARC_Queue_Pop(container->consoleKeyQueue);
        if(key == NULL){
            continue;
        }

        ARC_ConsoleKey_Destroy(key);
    }
}

HUSBANDO_TUIPage *HUSBANDO_TUIContainer_GetPage(HUSBANDO_TUIContainer *container){
    return container->page;
}

void HUSBANDO_TUIContainer_SetPage(HUSBANDO_TUIContainer *container, HUSBANDO_TUIPage *page){
    container->page = page;
}

void HUSBANDO_TUIContainer_AddPage(HUSBANDO_TUIContainer *container, HUSBANDO_TUIPageId pageId){
    container->nextPageId = pageId;
}

void HUSBANDO_TUIContainer_HandleNormalInput(HUSBANDO_TUIContainer *container){
    ARC_ConsoleKey *key = ARC_ConsoleView_GetCreateConsoleKeyAt(container->view, container->cursor);

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_CONSOLE_KEY_Q)){
        pthread_mutex_lock(&(container->bufferMutex));
        container->running = ARC_False;
        pthread_mutex_unlock(&(container->bufferMutex));
        return;
    }

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_CONSOLE_KEY_FORWARD_SLASH)){
        ARC_ConsoleView_SetCursorVisibility(container->view, ARC_CONSOLE_VIEW_CURSOR_VISIBLE);
        container->visibleCursor = ARC_True;
        container->inputMode = SEARCH;
        return;
    }

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_CONSOLE_KEY_H)){
        HUSBANDO_Core_ControlsSeekLeft(husbando_core);
        return;
    }

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_CONSOLE_KEY_L)){
        HUSBANDO_Core_ControlsSeekRight(husbando_core);
        return;
    }

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_CONSOLE_KEY_P)){
        HUSBANDO_Core_ControlsPlay(husbando_core);
        return;
    }

    ARC_Queue_Push(container->consoleKeyQueue, (void *)key);
}

void HUSBANDO_TUIContainer_HandleSearchInput(HUSBANDO_TUIContainer *container){
    ARC_ConsoleKey *key = ARC_ConsoleView_GetCreateConsoleKeyAt(container->view, container->cursor);
    if(ARC_ConsoleKey_EqualsPointer(key, ARC_CONSOLE_KEY_ESC)){
        ARC_ConsoleView_SetCursorVisibility(container->view, ARC_CONSOLE_VIEW_CURSOR_HIDDEN);
        container->visibleCursor = ARC_False;
        container->inputMode = NORMAL;
        return;
    }

    uint64_t currentConsoleSearchStringLength = strlen(container->consoleSearchString->data);

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_CONSOLE_KEY_BACKSPACE)){
        if(currentConsoleSearchStringLength == 0){
            return;
        }

        container->consoleSearchString->data[currentConsoleSearchStringLength - 1] = '\0';
        return;
    }

    char keyChar = ARC_ConsoleKey_GetCharFromKey(key);
    if(currentConsoleSearchStringLength < container->consoleSearchString->length){
        container->consoleSearchString->data[currentConsoleSearchStringLength] = keyChar;
    }
}
