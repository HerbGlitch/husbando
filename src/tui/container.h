#ifndef HUSBANDO_TUI_CONTAINER_H_
#define HUSBANDO_TUI_CONTAINER_H_

#include <arc/console/buffer.h>
#include <arc/console/view.h>
#include <arc/math/point.h>
#include <arc/std/bool.h>
#include <pthread.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @breif a callback function for the tui container, used to update the buffer in the background (like for video time)
 *
 * @param buffer     the main container buffer to update
 * @param updateView a boolean to signal if the view should update right away (recommended if the buffer changed)
 * @param userdata   data that should be passed into the function that has this function as a callback, can be NULL
*/
typedef void (* HUSBANDO_TUIContainer_PollFn)(ARC_ConsoleBuffer *buffer, ARC_Bool *updateView, void *userdata);

/**
 * @breif the tui container type, basically holds all of the tui, will be multithreaded to allow for background refreshing
*/
typedef struct HUSBANDO_TUIContainer {
    ARC_ConsoleView   *view;
    ARC_ConsoleBuffer *buffer;

    ARC_Point cursor;

    ARC_Bool updateView;

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
*/
void HUSBANDO_TUIContainer_Create(HUSBANDO_TUIContainer **container, char *title);

/**
 * @breif frees allocated memory in the tui container and deletes the container
 *
 * @param container the tui container to destroy
*/
void HUSBANDO_TUIContainer_Destory(HUSBANDO_TUIContainer *container);

/**
 * @breif runs the container on two threads, one for a page and one for a poll function
 *
 * @param container the tui container that will run
 * @param pollTime  the time in miliseconds to run the pull callback on
 * @param pollFn    a provided callback that can update the containers buffer
 * @param userdata  userdata that will be passed along to be used in the pollFn callback, can be NULL
*/
void HUSBANDO_TUIContainer_RunPageAndPoll(HUSBANDO_TUIContainer *container, uint32_t pollTime, HUSBANDO_TUIContainer_PollFn pollFn, void *userdata);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_TUI_CONTAINER_H_
