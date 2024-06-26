#ifndef HUSBANDO_TUI_CONTAINER_H_
#define HUSBANDO_TUI_CONTAINER_H_

#include "page.h"
#include <pthread.h>
#include <stdint.h>
#include <arc/console/view.h>
#include <arc/math/point.h>
#include <arc/std/bool.h>
#include <arc/std/stack.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @breif the tui container type, basically holds all of the tui, will be multithreaded to allow for background refreshing
*/
typedef struct HUSBANDO_TUIContainer {
    ARC_ConsoleView *view;
    ARC_Stack       *consoleKeyStack;

    char *title;

    HUSBANDO_TUIPage *page;

    ARC_Bool captureInput;

    ARC_Point cursor;
    ARC_Bool  visibleCursor;

    ARC_Bool running;

    uint32_t pollTime;

    pthread_t       pollThread;
    pthread_mutex_t bufferMutex;
} HUSBANDO_TUIContainer;

/**
 * @breif creates the main container of the tui, holds the console view, console buffer, and some multithreading things
 *
 * @note for now this will create the container with a border (and a title in the border if one is passed)
 *
 * @param container the tui container to create
 * @param title     the title of the container can be null
 * @param page      the initial page for the container, if null HUSBANDO_TUIContainer_RunPage will not work
 * @param pollTime  the time in miliseconds to pull the page data, set to zero to not poll the page
*/
void HUSBANDO_TUIContainer_Create(HUSBANDO_TUIContainer **container, char *title, HUSBANDO_TUIPage *page, uint32_t pollTime);

/**
 * @breif frees allocated memory in the tui container and deletes the container
 *
 * @param container the tui container to destroy
*/
void HUSBANDO_TUIContainer_Destory(HUSBANDO_TUIContainer *container);

/**
 * @breif runs the container page on two threads, one for a main function and one for a poll function
 *
 * @param container the tui container that will run its page
*/
void HUSBANDO_TUIContainer_RunPage(HUSBANDO_TUIContainer *container);

/**
 * @breif clears the console key buffer within a given tui container
 *
 * @param container the tui to clear the key buffer from
*/
void HUSBANDO_TUIContainer_ClearConsoleKeyStack(HUSBANDO_TUIContainer *container);

/**
 * @brief sets the tui containers page
 *
 * @param container the conter which is having its page being set
 * @param page      the page to set in the container, can be NULL
*/
void HUSBANDO_TUIContainer_SetPage(HUSBANDO_TUIContainer *container, HUSBANDO_TUIPage *page);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_TUI_CONTAINER_H_
