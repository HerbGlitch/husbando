#include "controls.h"

#include "core.h"

void HUSBANDO_Core_ControlsSeekLeft(HUSBANDO_Core *core){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return;
    }

    core->player->seekLeftFn(core);
}

void HUSBANDO_Core_ControlsSeekRight(HUSBANDO_Core *core){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return;
    }

    core->player->seekRightFn(core);
}

void HUSBANDO_Core_ControlsSetCurrentTime(HUSBANDO_Core *core, struct tm *timeinfo){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return;
    }

    core->player->setCurrentTimeFn(core, timeinfo);
}

struct tm *HUSBANDO_Core_ControlsGetTime(HUSBANDO_Core *core){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return NULL;
    }

    return core->player->getCurrentTimeFn(core);
}

struct tm *HUSBANDO_Core_ControlsFullTime(HUSBANDO_Core *core){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return NULL;
    }

    return core->player->getCurrentTimeFn(core);
}

void HUSBANDO_Core_ControlsSetAutoNext(HUSBANDO_Core *core, ARC_Bool autoNext){
    core->videoAutoNext = autoNext;
}

void HUSBANDO_Core_ControlsSetLoop(HUSBANDO_Core *core, uint64_t loop){
    core->videoLoop = loop;
}


