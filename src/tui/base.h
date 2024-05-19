#ifndef HUSBANDO_TUI_BASE_H_
#define HUSBANDO_TUI_BASE_H_

#include <time.h>
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
    ARC_ConsoleView *view;
    ARC_Time currentTime;
    ARC_Time fullTime;
} HUSBANDO_TUIBase;

/**
 * @breif creates the main outline of the tui, things like the playing video, skipping buttons, search, etc.
 *
 * @param base the tui base to create
*/
void HUSBANDO_TUIBase_Create(HUSBANDO_TUIBase **base, ARC_Rect bounds, char *title);

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
void HUSBANDO_TUIBase_RenderARCTime(HUSBANDO_TUIBase *base, ARC_Time time, ARC_Point pos);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_TUI_BASE_H_
