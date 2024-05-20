#ifndef HUSBANDO_TUI_PAGES_INDEX_H_
#define HUSBANDO_TUI_PAGES_INDEX_H_

#include "tui/container.h"
#include "tui/page.h"
#include <arc/math/rectangle.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @breif creates a page with index data (the main page data)
 *
 * @param page      the tui index page to create
 * @param container the container the index page will be displayed in, this will be stored as page->data
*/
void HUSBANDO_TUIPage_CreateIndex(HUSBANDO_TUIPage **page, HUSBANDO_TUIContainer *container, ARC_Rect bounds);

/**
 * @breif frees allocated memory in the tui index page and deletes the page
 *
 * @param page the tui index page to destroy
*/
void HUSBANDO_TUIPage_DestroyIndex(HUSBANDO_TUIPage *page);

/**
 * @breif a very basic page main function callback, used for interacting with a page and switching the page (in a game enigne think update)
 *
 * @note can and probably should also update the buffer and render, if the poll function is the only function used for rendering the information might lag a fair bit
 *
 * @param view page view matching the page this callback is called from
 * @param data page data matching the page this callback is called from
*/
void HUSBANDO_TUIPage_MainIndex(ARC_ConsoleView *view, void *data);

/**
 * @breif a very basic page poll function callback, used for updating the page content in the background
 *
 * @param view page view matching the page this callback is called from
 * @param data page data matching the page this callback is called from
*/
void HUSBANDO_TUIPage_PollIndex(ARC_ConsoleView *view, void *data);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_TUI_PAGES_INDEX_H_
