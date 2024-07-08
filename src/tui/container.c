#include "container.h"

#include "page.h"
#include "core/controls.h"
#include "core/core.h"
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <arc/console/key.h>
#include <arc/console/view.h>
#include <arc/math/point.h>
#include <arc/math/rectangle.h>
#include <arc/std/bool.h>
#include <arc/std/errno.h>
#include <arc/std/stack.h>
#include <arc/std/string.h>

void HUSBANDO_TUIContainer_Create(HUSBANDO_TUIContainer **container, char *title, HUSBANDO_TUIPage *page, uint32_t pollTime){
    *container = (HUSBANDO_TUIContainer *)malloc(sizeof(HUSBANDO_TUIContainer));

    ARC_ConsoleView_Create(&((*container)->view), (ARC_Rect){ 0, 0, 0, 0 });
    ARC_Stack_Create(&((*container)->consoleKeyStack));
    ARC_String_Create(&((*container)->consoleSearchString), "", 0);

    (*container)->title = title;

    (*container)->page = page;

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
        HUSBANDO_TUIContainer_ClearConsoleKeyStack(*container);

        ARC_Stack_Destroy((*container)->consoleKeyStack);
        ARC_ConsoleView_Destroy((*container)->view);

        free(*container);
        *container = NULL;
        return;
    }
}

void HUSBANDO_TUIContainer_Destory(HUSBANDO_TUIContainer *container){
    HUSBANDO_TUIContainer_ClearConsoleKeyStack(container);

    ARC_Stack_Destroy(container->consoleKeyStack);
    ARC_ConsoleView_Destroy(container->view);

    free(container);
}

//private type for polling
typedef struct HUSBANDO_TUIContainerPollParams {
    HUSBANDO_TUIPage_PollFn pollFn;
    ARC_ConsoleView *view;
    void *data;

    ARC_Bool *running;
    uint32_t pollTime;

    pthread_mutex_t *bufferMutex;
} HUSBANDO_TUIContainerPollParams;

//private function that runs the polling
void *HUSBANDO_TUIContainer_RunPoll(void *data){
    HUSBANDO_TUIContainerPollParams *pollParams = (HUSBANDO_TUIContainerPollParams *)data;

    pthread_mutex_lock(pollParams->bufferMutex);
    ARC_Bool threadRunning = *(pollParams->running);
    struct timespec sleepTime = { pollParams->pollTime / 1000, (pollParams->pollTime % 1000) * 1000000 };
    pthread_mutex_unlock(pollParams->bufferMutex);

    while(threadRunning){
        pthread_mutex_lock(pollParams->bufferMutex);
        pollParams->pollFn(pollParams->view, pollParams->data);
        pthread_mutex_unlock(pollParams->bufferMutex);

        nanosleep(&sleepTime, &sleepTime);

        pthread_mutex_lock(pollParams->bufferMutex);
        threadRunning = *(pollParams->running);
        pthread_mutex_unlock(pollParams->bufferMutex);
    }

    return NULL;
}

void HUSBANDO_TUIContainer_RunPage(HUSBANDO_TUIContainer *container){
    if(container->page == NULL){
        arc_errno = ARC_ERRNO_NULL;
        ARC_DEBUG_ERR("HUSBANDO_TUIContainer_RunPage(container), page was NULL");
        return;
    }

    HUSBANDO_TUIContainerPollParams pollParams = {
        container->page->pollFn,
        container->page->view,
        container->page->data,

        &(container->running),
        container->pollTime,

        &(container->bufferMutex)
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
    container->running = ARC_True;
    while(container->running){
        //handle input modes
        switch(container->inputMode){
            case NORMAL:
                HUSBANDO_TUIContainer_HandleNormalInput(container);
                break;

            case SEARCH:
                HUSBANDO_TUIContainer_HandleSearchInput(container);
                break;
        }

        //update the page with the new input
        pthread_mutex_lock(&(container->bufferMutex));
        container->page->mainFn(container->page->view, container->page->data);
        pthread_mutex_unlock(&(container->bufferMutex));
    }

    //join the threads on end
    pthread_join(container->pollThread, NULL);
}

void HUSBANDO_TUIContainer_ClearConsoleKeyStack(HUSBANDO_TUIContainer *container){
    while(ARC_Stack_Size(container->consoleKeyStack) != 0){
        ARC_ConsoleKey *key = (ARC_ConsoleKey *)ARC_Stack_Pop(container->consoleKeyStack);
        if(key == NULL){
            continue;
        }

        ARC_ConsoleKey_Destroy(key);
    }
}

void HUSBANDO_TUIContainer_SetPage(HUSBANDO_TUIContainer *container, HUSBANDO_TUIPage *page){
    container->page = page;
}

void HUSBANDO_TUIContainer_HandleNormalInput(HUSBANDO_TUIContainer *container){
    ARC_ConsoleKey *key = ARC_ConsoleView_GetCreateConsoleKeyAt(container->view, container->cursor);

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_KEY_Q)){
        pthread_mutex_lock(&(container->bufferMutex));
        container->running = ARC_False;
        pthread_mutex_unlock(&(container->bufferMutex));
        return;
    }

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_KEY_S)){
        container->inputMode = SEARCH;
        return;
    }

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_KEY_O)){
        ARC_String *url;
        //ARC_String_CreateWithStrlen(&url, "https://youtu.be/1P5BSm_oFJg");
        //ARC_String_CreateWithStrlen(&url, "https://youtu.be/fBGSJ3sbivI");
        //ARC_String_CreateWithStrlen(&url, "https://youtu.be/QdabIfmcqSQ");
        //ARC_String_CreateWithStrlen(&url, "https://youtu.be/QBYr0k8dOtw");
        ARC_String_CreateWithStrlen(&url, "https://youtu.be/RjM8d0Csuk4");
        HUSBANDO_Core_ControlsInit(husbando_core, url, ARC_True);
        ARC_String_Destroy(url);
        return;
    }

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_KEY_H)){
        HUSBANDO_Core_ControlsSeekLeft(husbando_core);
        return;
    }

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_KEY_L)){
        HUSBANDO_Core_ControlsSeekRight(husbando_core);
        return;
    }

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_KEY_P)){
        HUSBANDO_Core_ControlsPlay(husbando_core);
        return;
    }
}

void HUSBANDO_TUIContainer_HandleSearchInput(HUSBANDO_TUIContainer *container){
    ARC_ConsoleKey *key = ARC_ConsoleView_GetCreateConsoleKeyAt(container->view, container->cursor);
    if(ARC_ConsoleKey_EqualsPointer(key, ARC_KEY_ESC)){
        container->inputMode = NORMAL;
        return;
    }

    if(ARC_ConsoleKey_EqualsPointer(key, ARC_KEY_BACKSPACE)){
        if(container->consoleSearchString->length == 1){
            ARC_String_Destroy(container->consoleSearchString);
            ARC_String_Create(&(container->consoleSearchString), "", 0);
            return;
        }

        if(container->consoleSearchString->length == 0){
            return;
        }

        ARC_String_ReplaceWithSubstring(&(container->consoleSearchString), 0, container->consoleSearchString->length - 1);
        return;
    }

    char keyChar = ARC_ConsoleKey_GetCharFromKey(key);
    ARC_String_AppendCString(&(container->consoleSearchString), &keyChar, 1);
}
