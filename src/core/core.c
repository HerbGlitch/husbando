#include "core.h"

#include <stdlib.h>
#include <arc/std/string.h>

void HUSBANDO_Core_Create(HUSBANDO_Core **core, uint8_t playerId){
    *core = (HUSBANDO_Core *)malloc(sizeof(HUSBANDO_Core));

    //TODO: create ssh if ini is set to default connection
    (*core)->ssh = NULL;

    (*core)->videoName   = NULL;
    (*core)->videoUrl    = NULL;

    //TODO: get these from the ini
    (*core)->videoAutoNext = ARC_False;
    (*core)->videoLoop     = 0;

    (*core)->player     = NULL;
    (*core)->playerId   = 0;
    (*core)->playerData = NULL;
    HUSBANDO_Core_SetPlayer(*core, playerId);
}

void HUSBANDO_Core_Destroy(HUSBANDO_Core *core){
    HUSBANDO_Core_SetPlayer(core, HUSBANDO_CORE_VIDEO_PLAYER_NONE);

    if(core->videoName != NULL){
        ARC_String_Destroy(core->videoName);
    }

    if(core->videoUrl != NULL){
        ARC_String_Destroy(core->videoUrl);
    }

    free(core);
}

void HUSBANDO_Core_SetPlayer(HUSBANDO_Core *core, uint8_t playerId){
    //clean up last player's data
    switch(playerId){
        case HUSBANDO_CORE_VIDEO_PLAYER_NONE:
            break;
        case HUSBANDO_CORE_VIDEO_PLAYER_MPV:
            break;
        case HUSBANDO_CORE_VIDEO_PLAYER_CVLC:
            break;
    }

    //set the id to the new player id
    core->playerId = playerId;

    //set new player's data
    switch(playerId){
        case HUSBANDO_CORE_VIDEO_PLAYER_NONE:
            break;
        case HUSBANDO_CORE_VIDEO_PLAYER_MPV:
            break;
        case HUSBANDO_CORE_VIDEO_PLAYER_CVLC:
            break;
    }
}

void HUSBANDO_Core_SetSsh(HUSBANDO_Core *core, ARC_Bool sshConnect){
    
}
