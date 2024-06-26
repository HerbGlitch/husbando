#include "base.h"

#include "core/core.h"
#include "core/controls.h"
#include <stdlib.h>
#include <time.h>
#include <arc/console/view.h>
#include <arc/math/rectangle.h>
#include <arc/std/string.h>
#include <arc/std/time.h>

void HUSBANDO_TUIBase_Create(HUSBANDO_TUIBase **base, ARC_Rect bounds, char *title){
    *base = (HUSBANDO_TUIBase *)malloc(sizeof(HUSBANDO_TUIBase));

    ARC_ConsoleView_Create(&((*base)->view), bounds);

    (*base)->currentTime = (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    (*base)->fullTime = (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    //draw the header
    ARC_ConsoleView_SetBorder((*base)->view, ARC_CONSOLE_VIEW_BORDER_DEFAULT);
    if(title != NULL){
        ARC_String *titleString;
        ARC_String_CreateWithStrlen(&titleString, title);
        ARC_ConsoleView_RenderStringAt((*base)->view, titleString, (ARC_Point){ 2, 0 });
        ARC_String_Destroy(titleString);
    }

    //replace the bottom corners
    ARC_ConsoleView_RenderWCharAt((*base)->view, L'├', (ARC_Point){ 0, bounds.h - bounds.y - 1 });
    ARC_ConsoleView_RenderWCharAt((*base)->view, L'┤', (ARC_Point){ bounds.w - bounds.x - 1, bounds.h - bounds.y - 1 });
}

void HUSBANDO_TUIBase_Destory(HUSBANDO_TUIBase *base){
    ARC_ConsoleView_Destroy(base->view);

    free(base);
}

void HUSBANDO_TUIBase_Main(HUSBANDO_TUIBase *base){
    //title
    ARC_Rect bounds = ARC_ConsoleView_GetBounds(base->view);
    ARC_ConsoleView_RenderRect(base->view, (ARC_Rect){ bounds.x + 2, bounds.y + 1, 15, 3 });

    ARC_String *titleString;
    ARC_String_CreateWithStrlen(&titleString, "VIDEO TITLE");
    ARC_ConsoleView_RenderStringAt(base->view, titleString, (ARC_Point){ bounds.x + 4, bounds.y + 2 });
    ARC_String_Destroy(titleString);

    //time
    ARC_ConsoleView_RenderRect(base->view, (ARC_Rect){ bounds.x + 16, bounds.y + 1, 21, 3 });
    ARC_ConsoleView_RenderWCharAt(base->view, L'┬', (ARC_Point){ bounds.x + 16, bounds.y + 1 });
    ARC_ConsoleView_RenderWCharAt(base->view, L'┴', (ARC_Point){ bounds.x + 16, bounds.y + 3 });

    //get the current player time and the full time
    base->currentTime = HUSBANDO_Core_ControlsGetCurrentTime(husbando_core);
    base->fullTime    = HUSBANDO_Core_ControlsGetFullTime(husbando_core);

    HUSBANDO_TUIBase_RenderARCTime(base, base->currentTime, (ARC_Point){ bounds.x + 18, bounds.y + 2 });
    ARC_ConsoleView_RenderCharAt(base->view, '/', (ARC_Point){ bounds.x + 26, bounds.y + 2 });
    HUSBANDO_TUIBase_RenderARCTime(base, base->fullTime, (ARC_Point){ bounds.x + 27, bounds.y + 2 });

    //controls
    ARC_ConsoleView_RenderRect(base->view, (ARC_Rect){ bounds.x + 36, bounds.y + 1, 5, 3 });
    ARC_ConsoleView_RenderWCharAt(base->view, L'┬', (ARC_Point){ bounds.x + 36, bounds.y + 1 });
    ARC_ConsoleView_RenderWCharAt(base->view, L'┴', (ARC_Point){ bounds.x + 36, bounds.y + 3 });
    ARC_ConsoleView_RenderCharAt(base->view, '<', (ARC_Point){ bounds.x + 38, bounds.y + 2 });

    ARC_ConsoleView_RenderRect(base->view, (ARC_Rect){ bounds.x + 40, bounds.y + 1, 5, 3 });
    ARC_ConsoleView_RenderWCharAt(base->view, L'┬', (ARC_Point){ bounds.x + 40, bounds.y + 1 });
    ARC_ConsoleView_RenderWCharAt(base->view, L'┴', (ARC_Point){ bounds.x + 40, bounds.y + 3 });
    ARC_ConsoleView_RenderCharAt(base->view, '=', (ARC_Point){ bounds.x + 42, bounds.y + 2 });

    ARC_ConsoleView_RenderRect(base->view, (ARC_Rect){ bounds.x + 44, bounds.y + 1, 5, 3 });
    ARC_ConsoleView_RenderWCharAt(base->view, L'┬', (ARC_Point){ bounds.x + 44, bounds.y + 1 });
    ARC_ConsoleView_RenderWCharAt(base->view, L'┴', (ARC_Point){ bounds.x + 44, bounds.y + 3 });
    ARC_ConsoleView_RenderCharAt(base->view, '>', (ARC_Point){ bounds.x + 46, bounds.y + 2 });

    //search
    ARC_ConsoleView_RenderRect(base->view, (ARC_Rect){ bounds.x + bounds.w - 74, bounds.y + 1, 10, 3 });
    ARC_ConsoleView_RenderCStringWithStrlenAt(base->view, "SEARCH", (ARC_Point){ bounds.x + bounds.w - 72, bounds.y + 2 });

    ARC_ConsoleView_RenderRect(base->view, (ARC_Rect){ bounds.x + bounds.w - 65, bounds.y + 1, 40, 3 });
    ARC_ConsoleView_RenderWCharAt(base->view, L'┬', (ARC_Point){ bounds.x + bounds.w - 65, bounds.y + 1 });
    ARC_ConsoleView_RenderWCharAt(base->view, L'┴', (ARC_Point){ bounds.x + bounds.w - 65, bounds.y + 3 });

    //sort
    ARC_ConsoleView_RenderRect(base->view, (ARC_Rect){ bounds.x + bounds.w - 24, bounds.y + 1, 10, 3 });
    ARC_ConsoleView_RenderCStringWithStrlenAt(base->view, "SORT", (ARC_Point){ bounds.x + bounds.w - 22, bounds.y + 2 });
    ARC_ConsoleView_RenderCharAt(base->view, 'v', (ARC_Point){ bounds.x + bounds.w - 17, bounds.y + 2 });

    //ssh
    ARC_ConsoleView_RenderRect(base->view, (ARC_Rect){ bounds.x + bounds.w - 13, bounds.y + 1, 11, 3 });
    ARC_ConsoleView_RenderCStringWithStrlenAt(base->view, "SSH", (ARC_Point){ bounds.x + bounds.w - 9, bounds.y + 2 });

}

void HUSBANDO_TUIBase_PollIndex(HUSBANDO_TUIBase *base){
    ARC_Rect bounds = ARC_ConsoleView_GetBounds(base->view);

    ARC_Time oldCurrentTime = base->currentTime;
    ARC_Time oldFullTime = base->fullTime;

    //get the current player time and the full time
    base->currentTime = HUSBANDO_Core_ControlsGetCurrentTime(husbando_core);
    base->fullTime    = HUSBANDO_Core_ControlsGetFullTime(husbando_core);

    if(oldCurrentTime.seconds != base->currentTime.seconds){
        HUSBANDO_TUIBase_RenderARCTime(base, base->currentTime, (ARC_Point){ bounds.x + 18, bounds.y + 2 });
    }

    if(oldFullTime.seconds != base->fullTime.seconds){
        HUSBANDO_TUIBase_RenderARCTime(base, base->fullTime, (ARC_Point){ bounds.x + 27, bounds.y + 2 });
    }
}

void HUSBANDO_TUIBase_RenderARCTime(HUSBANDO_TUIBase *base, ARC_Time time, ARC_Point pos){
    ARC_Rect bounds = ARC_ConsoleView_GetBounds(base->view);
    int32_t zeroOffset = 0;

    if(time.hour < 10){
        zeroOffset = 1;
        ARC_ConsoleView_RenderCharAt(base->view, '0', (ARC_Point){ pos.x, bounds.y + 2 });
    }
    ARC_ConsoleView_RenderUint32At(base->view, time.hour, (ARC_Point){ pos.x + zeroOffset, pos.y });
    zeroOffset = 0;

    ARC_ConsoleView_RenderCharAt(base->view, ':', (ARC_Point){ pos.x + 2, bounds.y + 2 });

    if(time.minutes < 10){
        zeroOffset = 1;
        ARC_ConsoleView_RenderCharAt(base->view, '0', (ARC_Point){ pos.x + 3, bounds.y + 2 });
    }
    ARC_ConsoleView_RenderUint32At(base->view, time.minutes, (ARC_Point){ pos.x + 3 + zeroOffset, pos.y });
    zeroOffset = 0;

    ARC_ConsoleView_RenderCharAt(base->view, ':', (ARC_Point){ pos.x + 5, pos.y });

    if(time.seconds < 10){
        zeroOffset = 1;
        ARC_ConsoleView_RenderCharAt(base->view, '0', (ARC_Point){ pos.x + 6, bounds.y + 2 });
    }
    ARC_ConsoleView_RenderUint32At(base->view, time.seconds, (ARC_Point){ pos.x + 6 + zeroOffset, pos.y });
}
