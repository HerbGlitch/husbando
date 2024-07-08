#ifndef HUSBANDO_TUI_BASE_H_
#define HUSBANDO_TUI_BASE_H_

#include "container.h"
#include <arc/console/view.h>
#include <arc/math/rectangle.h>
#include <arc/std/time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @breif the tui base type, basically the reusable contents of a page (think navbar on a website)
*/
typedef struct HUSBANDO_TUIBase {
    HUSBANDO_TUIContainer *container;

    ARC_ConsoleView *view;
    ARC_Time currentTime;
    ARC_Time fullTime;

    ARC_String *search;
} HUSBANDO_TUIBase;

/**
 * @breif creates the main outline of the tui, things like the playing video, skipping buttons, search, etc.
 *
 * @param base      the tui base to create
 * @param container the container used for seach info and video stuff
 * @param bounds    the area to render the main outline in
 * @param title     the title of the application (probs husbando, but can be configurable)
*/
void HUSBANDO_TUIBase_Create(HUSBANDO_TUIBase **base, HUSBANDO_TUIContainer *container, ARC_Rect bounds, char *title);

/**
 * @breif frees alocated memory in the tui base and delete the base
 *
 * @param base the tui container to destroy
*/
void HUSBANDO_TUIBase_Destory(HUSBANDO_TUIBase *base);

/**
 * @breif
 *
 * @param
*/
void HUSBANDO_TUIBase_Main(HUSBANDO_TUIBase *base);

/**
 * @breif
 *
 * @param
*/
void HUSBANDO_TUIBase_PollIndex(HUSBANDO_TUIBase *base);

/**
 * @breif
 *
 * @param
*/
void HUSBANDO_TUIBase_RenderOutline(HUSBANDO_TUIBase *base, char *title);

/**
 * @breif
 *
 * @param
*/
void HUSBANDO_TUIBase_RenderARCTime(HUSBANDO_TUIBase *base, ARC_Time time, ARC_Point pos);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_TUI_BASE_H_
