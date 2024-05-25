#include "core.h"

#include "config.h"
#include "video/mpv/player.h"
#include <stdlib.h>
#include <arc/std/bool.h>
#include <arc/std/string.h>
#include <arc/networking/ssh.h>

HUSBANDO_Core *husbando_core = NULL;

void HUSBANDO_Core_Create(HUSBANDO_Core **core, uint8_t playerId){
    *core = (HUSBANDO_Core *)malloc(sizeof(HUSBANDO_Core));

    //TODO: create ssh if ini is set to default connection
    (*core)->ssh = NULL;
    if(husbando_config.Main.sshOnInit == 1){
        HUSBANDO_Core_SetSsh(*core, ARC_True);
    }

    (*core)->videoName   = NULL;
    (*core)->videoUrl    = NULL;

    //TODO: get these from the ini
    (*core)->videoAutoNext = ARC_False;
    (*core)->videoLoop     = 0;

    (*core)->player     = NULL;
    (*core)->playerId   = 0;
    HUSBANDO_Core_SetPlayer(*core, playerId);
}

void HUSBANDO_Core_Destroy(HUSBANDO_Core *core){
    HUSBANDO_Core_SetSsh(core, ARC_False);
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
    switch(core->playerId){
        case HUSBANDO_CORE_VIDEO_PLAYER_NONE:
            break;

        case HUSBANDO_CORE_VIDEO_PLAYER_MPV:
            HUSBANDO_CorePlayer_DestroyMPVPlayer(core->player);
            core->player = NULL;
            break;

        case HUSBANDO_CORE_VIDEO_PLAYER_CVLC:
            break;
    }

    //set the id to the new player id
    core->playerId = playerId;

    //set new player's data
    switch(core->playerId){
        case HUSBANDO_CORE_VIDEO_PLAYER_NONE:
            break;

        case HUSBANDO_CORE_VIDEO_PLAYER_MPV:
            HUSBANDO_CorePlayer_CreateMPVPlayer(&(core->player));
            break;

        case HUSBANDO_CORE_VIDEO_PLAYER_CVLC:
            break;
    }
}

void HUSBANDO_Core_SetSsh(HUSBANDO_Core *core, ARC_Bool sshConnect){
    if(!sshConnect && core->ssh != NULL){
        ARC_Ssh_Destroy(core->ssh);
        core->ssh = NULL;
        return;
    }

    ARC_Ssh_Create(&(core->ssh), (char *)husbando_config.Ssh.hostname, (char *)husbando_config.Ssh.user, (char *)husbando_config.Ssh.password);
}
