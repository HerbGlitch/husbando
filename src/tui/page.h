#ifndef HUSBANDO_TUI_PAGE_H_
#define HUSBANDO_TUI_PAGE_H_

#include <arc/console/view.h>
#include <arc/math/rectangle.h>
#include <arc/std/bool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @breif a very basic page main function callback, used for interacting with a page and switching the page (in a game enigne think update)
 *
 * @note can and probably should also update the buffer and render, if the poll function is the only function used for rendering the information might lag a fair bit
 *
 * @param view page view matching the page this callback is called from
 * @param data page data matching the page this callback is called from
*/
typedef void (* HUSBANDO_TUIPage_MainFn)(ARC_ConsoleView *view, void *data);

/**
 * @breif a very basic page poll function callback, used for updating the page content in the background
 *
 * @param view page view matching the page this callback is called from
 * @param data page data matching the page this callback is called from
*/
typedef void (* HUSBANDO_TUIPage_PollFn)(ARC_ConsoleView *view, void *data);

/**
 * @breif the very basic tui page type, basically holds everything held within the container
*/
typedef struct HUSBANDO_TUIPage {
    ARC_ConsoleView *view;
    void *data;

    HUSBANDO_TUIPage_MainFn mainFn;
    HUSBANDO_TUIPage_PollFn pollFn;
} HUSBANDO_TUIPage;

/**
 * @breif creates the a page in the tui, this is the main content of the tui
 *
 * @note data will be set to NULL
 *
 * @param page   the tui page to create
 * @param mainFn the main function callback for the page
 * @param pollFn the polling function callback for the page
*/
void HUSBANDO_TUIPage_Create(HUSBANDO_TUIPage **page, HUSBANDO_TUIPage_MainFn mainFn, HUSBANDO_TUIPage_PollFn pollFn, ARC_Rect bounds);

/**
 * @breif frees allocated memory in the tui page and deletes the page
 *
 * @note data will not be freed as it was not set in create
 *
 * @param page the tui page to destroy
*/
void HUSBANDO_TUIPage_Destroy(HUSBANDO_TUIPage *page);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_TUI_PAGE_H_
