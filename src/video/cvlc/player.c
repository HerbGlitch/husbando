#include "player.h"

#include "arc/std/string.h"
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
    free(player);
}

void HUSBANDO_CVLC_InitFn(HUSBANDO_Core *core, ARC_String *url, ARC_Bool autoPlay){
    //start the player with the given socket path in ssh
    if(core->ssh != NULL){
        char command[strlen(HUSBANDO_CVLC_SSH_COMMAND_START) + url->length + strlen(HUSBANDO_CVLC_SSH_COMMAND_END)];
        sprintf(command, "%s%s%s", HUSBANDO_CVLC_SSH_COMMAND_START, url->data, HUSBANDO_CVLC_SSH_COMMAND_END);
        ARC_Ssh_ExecStrInNewSession(core->ssh, command);
        return;
    }

    //start the player with the given socket path locally
    char command[strlen(HUSBANDO_CVLC_COMMAND_START) + url->length + strlen(HUSBANDO_CVLC_COMMAND_END)];
    sprintf(command, "%s%s%s", HUSBANDO_CVLC_COMMAND_START, url->data, HUSBANDO_CVLC_COMMAND_END);
    system(command);
}

void HUSBANDO_CVLC_PlayFn(HUSBANDO_Core *core){
    char *command = (char *)HUSBANDO_CVLC_PLAYER_PLAY;

    if(core->ssh != NULL){
        ARC_Ssh_ExecStrInNewSession(core->ssh, command);
        return;
    }

    system(command);
}

void HUSBANDO_CVLC_PauseFn(HUSBANDO_Core *core){
    char *command = (char *)HUSBANDO_CVLC_PLAYER_PAUSE;

    if(core->ssh != NULL){
        ARC_Ssh_ExecStrInNewSession(core->ssh, command);
        return;
    }

    system(command);
}

void HUSBANDO_CVLC_SeekRightFn(HUSBANDO_Core *core){
    char *command = (char *)HUSBANDO_CVLC_PLAYER_SEEK_RIGHT;

    if(core->ssh != NULL){
        ARC_Ssh_ExecStrInNewSession(core->ssh, command);
        return;
    }

    system(command);
}

void HUSBANDO_CVLC_SeekLeftFn(HUSBANDO_Core *core){
    char *command = (char *)HUSBANDO_CVLC_PLAYER_SEEK_LEFT;

    if(core->ssh != NULL){
        ARC_Ssh_ExecStrInNewSession(core->ssh, command);
        return;
    }

    system(command);
}

#define HUSBANDO_CVLC_PLAYER_GET_TIME_POSITION "playerctl position"
#define HUSBANDO_CVLC_PLAYER_GET_TOTAL_TIME    "playerctl metadata vlc:time"

ARC_Time HUSBANDO_CVLC_GetCurrentTimeFn(HUSBANDO_Core *core){
    ARC_Time currentTime = (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    if(core->currentEpisode == NULL){
        return currentTime;
    }

    char *command = HUSBANDO_CVLC_PLAYER_GET_TIME_POSITION;

    if(core->ssh != NULL){
        ARC_String *responseString = ARC_Ssh_ExecStrInNewSessionAndGetResponse(core->ssh, command);
        if(responseString == NULL){
            return currentTime;
        }

        uint32_t currentTimeInSeconds = (uint32_t)ARC_String_ToDouble(responseString);
        if(currentTimeInSeconds != 0){
            currentTime.seconds = currentTimeInSeconds % 60;
            currentTime.minutes = (currentTimeInSeconds / 60) % 60;
            currentTime.hour    = (currentTimeInSeconds / 60) / 60;
        }

        ARC_String_Destroy(responseString);
        return currentTime;
    }

//    FILE *file = popen(command, "r");
//    if(file == NULL){
//        // error processing and return
//        return (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//    }
//
//    char buffer[256] ;
//    while(fgets(buffer, 256, file) != NULL){
//        // process a line
//    }

    //TODO: get time from local player
    return currentTime;
}

ARC_Time HUSBANDO_CVLC_GetFullTimeFn(HUSBANDO_Core *core){
    ARC_Time currentTime = (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    if(core->currentEpisode == NULL){
        return currentTime;
    }

    char *command = HUSBANDO_CVLC_PLAYER_GET_TOTAL_TIME;

    if(core->ssh != NULL){
        ARC_String *responseString = ARC_Ssh_ExecStrInNewSessionAndGetResponse(core->ssh, command);
        if(responseString == NULL){
            return currentTime;
        }

        uint32_t currentTimeInSeconds = (uint32_t)ARC_String_ToUint64_t(responseString);
        if(currentTimeInSeconds != 0){
            currentTime.seconds = currentTimeInSeconds % 60;
            currentTime.minutes = (currentTimeInSeconds / 60) % 60;
            currentTime.hour    = (currentTimeInSeconds / 60) / 60;
        }

        ARC_String_Destroy(responseString);
        return currentTime;
    }

    //TODO: get time from local player
    return (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };}

void HUSBANDO_CVLC_SetCurrentTimeFn(HUSBANDO_Core *core, ARC_Time time){
}
