#ifndef ARC_NCURSES_H_
#define ARC_NCURSES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <arc/math/rectangle.h>
#include <arc/std/string.h>

extern uint8_t arc_ncurses_win_size;

/**
 * @brief
*/
typedef struct ARC_NCurses ARC_NCurses;

/**
 * @brief
*/
typedef struct ARC_NCursesElement ARC_NCursesElement;

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

/**
 * @brief border options
*/
#define ARC_NCURSES_ATTRIBUTE_NONE    0x00
#define ARC_NCURSES_ATTRIBUTE_REVERSE 0x01

/**
 * @brief sets a border on the ARC_NCurses
 *
 * @param ncurses ARC_NCurses to set border to
 * @param border  The border to set
*/
void ARC_NCurses_SetAttribute(ARC_NCurses *ncurses, uint32_t attribute);

/**
 * @brief adds an ARC_NCursesElement to the ARC_NCusres type
 *
 * @note the elements position will be based on the ARC_NCurses which might mess up how it looks
 *
 * @param ncurses
 * @param elment
*/
void ARC_NCurses_AddElement(ARC_NCurses *ncurses, ARC_NCursesElement *element);

/**
 * @brief adds an ARC_NCursesElement to the ARC_NCusres type
 *
 * @note the elements position will be based on the ARC_NCurses which might mess up how it looks
 *
 * @param ncurses
 * @param index
*/
ARC_NCursesElement *ARC_NCurses_GetElement(ARC_NCurses *ncurses, uint32_t index);

/**
 * @brief
 *
 * @param ncurses
 * @param index
*/
void ARC_NCurses_RemoveElement(ARC_NCurses *ncurses, uint32_t index);

/**
 * @brief
 *
 * @param ncurses
 * @param text
 * @param pos
*/
void ARC_NCurses_RenderStringAt(ARC_NCurses *ncurses, ARC_String *text, ARC_Point pos);

/**
 * @brief
 *
 * @param ncurses
*/
void ARC_NCurses_RenderElements(ARC_NCurses *ncurses);

#ifdef __cplusplus
}
#endif

#endif //!ARC_NCURSES_H_
