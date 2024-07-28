#include "player.h"

#include "config.h"
#include "core/core.h"
#include "core/player.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arc/std/bool.h>
#include <arc/std/time.h>

void HUSBANDO_CorePlayer_CreateCVLCPlayer(HUSBANDO_CorePlayer **player){
    *player = (HUSBANDO_CorePlayer *)malloc(sizeof(HUSBANDO_CorePlayer));

    (*player)->initFn           = HUSBANDO_CVLC_InitFn;
    (*player)->playFn           = HUSBANDO_CVLC_PlayFn;
    (*player)->pauseFn          = HUSBANDO_CVLC_PauseFn;
    (*player)->seekRightFn      = HUSBANDO_CVLC_SeekRightFn;
    (*player)->seekLeftFn       = HUSBANDO_CVLC_SeekLeftFn;
    (*player)->getCurrentTimeFn = HUSBANDO_CVLC_GetCurrentTimeFn;
    (*player)->getFullTimeFn    = HUSBANDO_CVLC_GetFullTimeFn;
    (*player)->setCurrentTimeFn = HUSBANDO_CVLC_SetCurrentTimeFn;

    (*player)->data = NULL;
}

void HUSBANDO_CorePlayer_DestroyCVLCPlayer(HUSBANDO_CorePlayer *player){
}

void HUSBANDO_CVLC_InitFn(HUSBANDO_Core *core, ARC_String *url, ARC_Bool autoPlay){
}

void HUSBANDO_CVLC_PlayFn(HUSBANDO_Core *core){
}

void HUSBANDO_CVLC_PauseFn(HUSBANDO_Core *core){
}

void HUSBANDO_CVLC_SeekRightFn(HUSBANDO_Core *core){
}

void HUSBANDO_CVLC_SeekLeftFn(HUSBANDO_Core *core){
}

ARC_Time HUSBANDO_CVLC_GetCurrentTimeFn(HUSBANDO_Core *core){
    return (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

ARC_Time HUSBANDO_CVLC_GetFullTimeFn(HUSBANDO_Core *core){
    return (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

void HUSBANDO_CVLC_SetCurrentTimeFn(HUSBANDO_Core *core, ARC_Time time){
}
