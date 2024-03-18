#ifndef ARC_NCURSES_ELEMENT_H_
#define ARC_NCURSES_ELEMENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ncurses.h"
#include "arc/std/string.h"
#include "arc/std/bool.h"
#include "arc/math/point.h"

/**
 * @brief
*/
typedef struct ARC_NCursesElement ARC_NCursesElement;

/**
 * @brief
*/
typedef void (*ARC_NCursesElement_RenderFn)(ARC_NCurses *ncurses, ARC_NCursesElement *element);

/**
 * @brief
*/
typedef struct ARC_NCursesElement {
    uint32_t type;
    uint8_t flags;
    ARC_String *string;

    ARC_Point pos;

    ARC_NCursesElement_RenderFn renderFn;
} ARC_NCursesElement;

/**
 * @brief
*/
#define ARC_NCURSES_ELEMENT_FLAG_NONE       0b00000000
#define ARC_NCURSES_ELEMENT_FLAG_SELECTABLE 0b00000001
#define ARC_NCURSES_ELEMENT_FLAG_SELECTED   0b00000010

/**
 * @brief
*/
void ARC_NCursesElement_Create(ARC_NCursesElement **element, uint32_t type, uint8_t flags, ARC_String *string, ARC_Point pos, ARC_NCursesElement_RenderFn renderFn);

/**
 * @brief
*/
void ARC_NCursesElement_Destroy(ARC_NCursesElement *element);

/**
 * @brief
*/
void ARC_NCursesElement_DefaultRenderFn(ARC_NCurses *ncurses, ARC_NCursesElement *element);

/**
 * @brief
 *
 * @param
*/
ARC_Bool ARC_NCursesElement_IsSelectable(ARC_NCursesElement *element);

/**
 * @brief
 *
 * @param
*/
void ARC_NCursesElement_SetSelected(ARC_NCursesElement *element, ARC_Bool selected);

/**
 * @brief
*/
void ARC_NCursesElement_ToggleSelected(ARC_NCursesElement *element);

#endif //!ARC_NCURSES_ELEMENT_H_
