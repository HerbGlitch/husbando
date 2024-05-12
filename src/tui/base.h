#ifndef HUSBANDO_TUI_BASE_H_
#define HUSBANDO_TUI_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @breif the tui base type, basically the reusable contents of a page (think navbar on a website)
*/
typedef struct HUSBANDO_TUIBase {
} HUSBANDO_TUIBase;

/**
 * @breif creates the main outline of the tui, things like the playing video, skipping buttons, search, etc.
 *
 * @param base the tui base to create
*/
void HUSBANDO_TUIBase_Create(HUSBANDO_TUIBase **base);

/**
 * @breif frees alocated memory in the tui base and delete the base
 *
 * @param base the tui container to destroy
*/
void HUSBANDO_TUIBase_Destory(HUSBANDO_TUIBase *base);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_TUI_BASE_H_
