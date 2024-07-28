#include "player.h"

#include "arc/networking/ssh.h"
#include "arc/std/string.h"
#include "config.h"
#include "core/core.h"
#include "core/player.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <json-c/json.h>
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
    //TODO: might need to disconnect from socket?
    HUSBANDO_MPV_Data *mpvData = (HUSBANDO_MPV_Data *)player->data;
    free(mpvData);

    free(player);
}

void HUSBANDO_MPV_InitFn(HUSBANDO_Core *core, ARC_String *url, ARC_Bool autoPlay){
    HUSBANDO_MPV_Data *mpvData = (HUSBANDO_MPV_Data *)core->player->data;
    if(mpvData->socketConnected){
        //TOOD Throw an error or smthn
        return;
    }

    //start the player with the given socket path in ssh
    if(core->ssh != NULL){
        char command[strlen(HUSBANDO_MPV_SSH_COMMAND_START) + url->length + strlen(HUSBANDO_MPV_COMMAND_SOCKET_FLAG) + strlen(husbando_config.Mpv.socketName) + strlen(HUSBANDO_MPV_SSH_COMMAND_END)];
        sprintf(command, "%s%s%s%s%s", HUSBANDO_MPV_SSH_COMMAND_START, url->data, HUSBANDO_MPV_COMMAND_SOCKET_FLAG, husbando_config.Mpv.socketName, HUSBANDO_MPV_SSH_COMMAND_END);
        ARC_Ssh_ExecStrInNewSession(core->ssh, command);
        return;
    }

    //start the player with the given socket path locally
    char command[strlen(HUSBANDO_MPV_COMMAND_START) + url->length + strlen(HUSBANDO_MPV_COMMAND_SOCKET_FLAG) + strlen(husbando_config.Mpv.socketName) + strlen(HUSBANDO_MPV_COMMAND_END)];
    sprintf(command, "%s%s%s%s%s", HUSBANDO_MPV_COMMAND_START, url->data, HUSBANDO_MPV_COMMAND_SOCKET_FLAG, husbando_config.Mpv.socketName, HUSBANDO_MPV_COMMAND_END);
    system(command);

    //try to connect to the socket
//    if(connect(mpvData->socketFileDescriptor, (const struct sockaddr*)&addr, sizeof(addr)) == -1) {
//        mpvData->socketConnected = ARC_False;
//    }
}

void HUSBANDO_MPV_PlayFn(HUSBANDO_Core *core){
    char command[strlen(HUSBANDO_MPV_PLAYER_PLAY_PAUSE) + strlen(husbando_config.Mpv.socketName)];
    sprintf(command, "%s%s", HUSBANDO_MPV_PLAYER_PLAY_PAUSE, husbando_config.Mpv.socketName);

    if(core->ssh != NULL){
        ARC_Ssh_ExecStrInNewSession(core->ssh, command);
        return;
    }

    system(command);
}

void HUSBANDO_MPV_PauseFn(HUSBANDO_Core *core){
    char command[strlen(HUSBANDO_MPV_PLAYER_PAUSE) + strlen(husbando_config.Mpv.socketName)];
    sprintf(command, "%s%s", HUSBANDO_MPV_PLAYER_PAUSE, husbando_config.Mpv.socketName);

    if(core->ssh != NULL){
        ARC_Ssh_ExecStrInNewSession(core->ssh, command);
        return;
    }

    system(command);
}

void HUSBANDO_MPV_SeekRightFn(HUSBANDO_Core *core){
    char command[strlen(HUSBANDO_MPV_PLAYER_SEEK_RIGHT) + strlen(husbando_config.Mpv.socketName)];
    sprintf(command, "%s%s", HUSBANDO_MPV_PLAYER_SEEK_RIGHT, husbando_config.Mpv.socketName);

    if(core->ssh != NULL){
        ARC_Ssh_ExecStrInNewSession(core->ssh, command);
        return;
    }

    system(command);
}

void HUSBANDO_MPV_SeekLeftFn(HUSBANDO_Core *core){
    char command[strlen(HUSBANDO_MPV_PLAYER_SEEK_LEFT) + strlen(husbando_config.Mpv.socketName)];
    sprintf(command, "%s%s", HUSBANDO_MPV_PLAYER_SEEK_LEFT, husbando_config.Mpv.socketName);

    if(core->ssh != NULL){
        ARC_Ssh_ExecStrInNewSession(core->ssh, command);
        return;
    }

    system(command);
}

ARC_Time HUSBANDO_MPV_GetCurrentTimeFn(HUSBANDO_Core *core){
    ARC_Time currentTime = (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    if(core->currentEpisode == NULL){
        return currentTime;
    }

    char command[strlen(HUSBANDO_MPV_PLAYER_GET_TIME_POSITION) + strlen(husbando_config.Mpv.socketName)];
    sprintf(command, "%s%s", HUSBANDO_MPV_PLAYER_GET_TIME_POSITION, husbando_config.Mpv.socketName);

    if(core->ssh != NULL){
        ARC_String *responseString = ARC_Ssh_ExecStrInNewSessionAndGetResponse(core->ssh, command);
        if(responseString == NULL){
            return currentTime;
        }

        json_object *root = json_tokener_parse(responseString->data);
        ARC_String_Destroy(responseString);
        if(!root){
            json_object_put(root);
            return currentTime;
        }

        json_object *secondsJson = json_object_object_get(root, "data");
        if(json_object_get_type(secondsJson) == json_type_null){
            json_object_put(root);
            return currentTime;
        }

        uint32_t currentTimeInSeconds = (uint32_t)json_object_get_int(secondsJson);
        if(currentTimeInSeconds != 0){
            currentTime.seconds = currentTimeInSeconds % 60;
            currentTime.minutes = (currentTimeInSeconds / 60) % 60;
            currentTime.hour    = (currentTimeInSeconds / 60) / 60;
        }

        json_object_put(root);

        return currentTime;
    }

    //TODO: get time from local player
    return (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

ARC_Time HUSBANDO_MPV_GetFullTimeFn(HUSBANDO_Core *core){
    ARC_Time currentTime = (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    if(core->currentEpisode == NULL){
        return currentTime;
    }

    char command[strlen(HUSBANDO_MPV_PLAYER_GET_TOTAL_TIME) + strlen(husbando_config.Mpv.socketName)];
    sprintf(command, "%s%s", HUSBANDO_MPV_PLAYER_GET_TOTAL_TIME, husbando_config.Mpv.socketName);

    if(core->ssh != NULL){
        ARC_String *responseString = ARC_Ssh_ExecStrInNewSessionAndGetResponse(core->ssh, command);
        if(responseString == NULL){
            return currentTime;
        }

        json_object *root = json_tokener_parse(responseString->data);
        ARC_String_Destroy(responseString);
        if(!root){
            json_object_put(root);
            return currentTime;
        }

        json_object *secondsJson = json_object_object_get(root, "data");
        if(json_object_get_type(secondsJson) == json_type_null){
            json_object_put(root);
            return currentTime;
        }

        uint32_t currentTimeInSeconds = (uint32_t)json_object_get_int(secondsJson);
        if(currentTimeInSeconds != 0){
            currentTime.seconds = currentTimeInSeconds % 60;
            currentTime.minutes = (currentTimeInSeconds / 60) % 60;
            currentTime.hour    = (currentTimeInSeconds / 60) / 60;
        }

        json_object_put(root);

        return currentTime;
    }

    //TODO: get time from local player
    return (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

void HUSBANDO_MPV_SetCurrentTimeFn(HUSBANDO_Core *core, ARC_Time time){
}
