#include "player.h"

#include "core/core.h"
#include "core/player.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arc/std/bool.h>
#include <arc/std/time.h>

typedef struct HUSBANDO_MPV_Data {
    int32_t  socketFileDescriptor;
    ARC_Bool socketConnected;
} HUSBANDO_MPV_Data;

void HUSBANDO_CorePlayer_CreateMPVPlayer(HUSBANDO_CorePlayer **player){
    *player = (HUSBANDO_CorePlayer *)malloc(sizeof(HUSBANDO_CorePlayer));

    (*player)->initFn           = HUSBANDO_MPV_InitFn;
    (*player)->playFn           = HUSBANDO_MPV_PlayFn;
    (*player)->pauseFn          = HUSBANDO_MPV_PauseFn;
    (*player)->seekRightFn      = HUSBANDO_MPV_SeekRightFn;
    (*player)->seekLeftFn       = HUSBANDO_MPV_SeekLeftFn;
    (*player)->getCurrentTimeFn = HUSBANDO_MPV_GetCurrentTimeFn;
    (*player)->getFullTimeFn    = HUSBANDO_MPV_GetFullTimeFn;
    (*player)->setCurrentTimeFn = HUSBANDO_MPV_SetCurrentTimeFn;

    HUSBANDO_MPV_Data *mpvData = (HUSBANDO_MPV_Data *)malloc(sizeof(HUSBANDO_MPV_Data));
    mpvData->socketFileDescriptor = socket(PF_UNIX, SOCK_STREAM, 0);
    mpvData->socketConnected = ARC_False;

    (*player)->data = (void *)mpvData;
}

void HUSBANDO_CorePlayer_DestroyMPVPlayer(HUSBANDO_CorePlayer *player){
}

void HUSBANDO_MPV_InitFn(HUSBANDO_Core *core, ARC_String *url, ARC_Bool autoPlay){
    HUSBANDO_MPV_Data *mpvData = (HUSBANDO_MPV_Data *)core->player->data;
    if(mpvData->socketConnected){
        //TOOD Throw an error or smthn
        return;
    }

    //TODO: add ssh capability

    //start the player with the given socket path
    //TODO: clean this up
    char systemCommand[strlen("mpv ") + url->length + strlen(" --input-ipc-server=") + strlen(HUSBANDO_MPV_SOCKET_PATH) + strlen(" --no-terminal") + 1];
    strcpy(systemCommand                                                          , "mpv ");
    strcpy(systemCommand + 4                                                      , url->data);
    strcpy(systemCommand + 4 + url->length                                        , " --input-ipc-server=");
    strcpy(systemCommand + 4 + url->length + 20                                   , HUSBANDO_MPV_SOCKET_PATH);
    strcpy(systemCommand + 4 + url->length + 20 + strlen(HUSBANDO_MPV_SOCKET_PATH), " --no-terminal");
    systemCommand[4 + url->length + 20 + strlen(HUSBANDO_MPV_SOCKET_PATH) + strlen(" --no-terminal") + 1] = '\0';
    system(systemCommand);

    //try to connect to the socket
//    if(connect(mpvData->socketFileDescriptor, (const struct sockaddr*)&addr, sizeof(addr)) == -1) {
//        mpvData->socketConnected = ARC_False;
//    }
}

void HUSBANDO_MPV_PlayFn(HUSBANDO_Core *core){
}

void HUSBANDO_MPV_PauseFn(HUSBANDO_Core *core){
}

void HUSBANDO_MPV_SeekRightFn(HUSBANDO_Core *core){
}

void HUSBANDO_MPV_SeekLeftFn(HUSBANDO_Core *core){
}

ARC_Time HUSBANDO_MPV_GetCurrentTimeFn(HUSBANDO_Core *core){
    return (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

ARC_Time HUSBANDO_MPV_GetFullTimeFn(HUSBANDO_Core *core){
    return (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

void HUSBANDO_MPV_SetCurrentTimeFn(HUSBANDO_Core *core, ARC_Time time){
}
