#include "element.h"
#include "arc/ncurses/ncurses.h"

#include <stdlib.h>
#include <ncurses.h>

void ARC_NCursesElement_Create(ARC_NCursesElement **element, uint32_t type, uint8_t flags, ARC_String *string, ARC_Point pos, ARC_NCursesElement_RenderFn renderFn){
    *element = (ARC_NCursesElement *)malloc(sizeof(ARC_NCursesElement));

    (*element)->type     = type;
    (*element)->flags    = flags;
    (*element)->string   = string;
    (*element)->pos      = pos;
    (*element)->renderFn = renderFn;
}

void ARC_NCursesElement_Destroy(ARC_NCursesElement *element){
    free(element);
}

void ARC_NCursesElement_DefaultRenderFn(ARC_NCurses *ncurses, ARC_NCursesElement *element){
    if(element->flags & ARC_NCURSES_ELEMENT_FLAG_SELECTED){
        ARC_NCurses_SetAttribute(ncurses, ARC_NCURSES_ATTRIBUTE_REVERSE);
    }

    ARC_NCurses_RenderStringAt(ncurses, element->string, element->pos);

    if(element->flags & ARC_NCURSES_ELEMENT_FLAG_SELECTED){
        ARC_NCurses_SetAttribute(ncurses, ARC_NCURSES_ATTRIBUTE_NONE);
    }
}

ARC_Bool ARC_NCursesElement_IsSelectable(ARC_NCursesElement *element){
    if(element->flags & ARC_NCURSES_ELEMENT_FLAG_SELECTABLE){
        return ARC_True;
    }

    return ARC_False;
}

void ARC_NCursesElement_SetSelected(ARC_NCursesElement *element, ARC_Bool selected){
    if(selected){
        element->flags |= ARC_NCURSES_ELEMENT_FLAG_SELECTED;
        return;
    }

    element->flags &= ~ARC_NCURSES_ELEMENT_FLAG_SELECTED;
}

void ARC_NCursesElement_ToggleSelected(ARC_NCursesElement *element){
    element->flags ^= ARC_NCURSES_ELEMENT_FLAG_SELECTED;
}
