#ifndef ARC_NCURSES_H_
#define ARC_NCURSES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <arc/math/rectangle.h>

extern uint8_t arc_ncurses_win_size;

/**
 * @brief
*/
typedef struct ARC_NCurses ARC_NCurses;

/**
 * @brief creates ARC_NCurses type
 *
 * @param ncurses      ARC_NCurses to create
 * @param windowBounds the bounds of the window, if NULL will take up the entire screen
*/
void ARC_NCurses_Create(ARC_NCurses **ncurses, ARC_Rect *windowBounds);

/**
 * @brief destroyes ARC_NCurses type
 *
 * @param ncurses ARC_NCurses to destroy
*/
void ARC_NCurses_Destroy(ARC_NCurses *ncurses);

/**
 * @brief border options
*/
#define ARC_NCURSES_BORDER_NONE    0x00
#define ARC_NCURSES_BORDER_DEFAULT 0x01

/**
 * @brief sets a border on the ARC_NCurses
 *
 * @param ncurses ARC_NCurses to set border to
 * @param border  The border to set
*/
void ARC_NCurses_SetBorder(ARC_NCurses *ncurses, uint32_t border);

#ifdef __cplusplus
}
#endif

#endif //!ARC_NCURSES_H_
