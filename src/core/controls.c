#include "controls.h"

#include "core.h"

void HUSBANDO_Core_ControlsInitFn(HUSBANDO_Core *core, ARC_String *url, ARC_Bool autoPlay){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return;
    }

    core->player->initFn(core, url, autoPlay);
}

void HUSBANDO_Core_ControlsPlayFn(HUSBANDO_Core *core){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return;
    }

    core->player->playFn(core);
}

void HUSBANDO_Core_ControlsPauseFn(HUSBANDO_Core *core){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return;
    }

    core->player->pauseFn(core);
}

void HUSBANDO_Core_ControlsSeekRight(HUSBANDO_Core *core){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return;
    }

    core->player->seekRightFn(core);
}

void HUSBANDO_Core_ControlsSeekLeft(HUSBANDO_Core *core){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return;
    }

    core->player->seekLeftFn(core);
}

ARC_Time HUSBANDO_Core_ControlsGetCurrentTime(HUSBANDO_Core *core){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    }

    return core->player->getCurrentTimeFn(core);
}

ARC_Time HUSBANDO_Core_ControlsGetFullTime(HUSBANDO_Core *core){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    }

    return core->player->getFullTimeFn(core);
}

void HUSBANDO_Core_ControlsSetCurrentTime(HUSBANDO_Core *core, ARC_Time time){
    if(core->playerId == HUSBANDO_CORE_VIDEO_PLAYER_NONE || core->player == NULL){
        //TODO: notify no player exists
        return;
    }

    core->player->setCurrentTimeFn(core, time);
}

void HUSBANDO_Core_ControlsSetAutoNext(HUSBANDO_Core *core, ARC_Bool autoNext){
    core->videoAutoNext = autoNext;
}

void HUSBANDO_Core_ControlsSetLoop(HUSBANDO_Core *core, uint64_t loop){
    core->videoLoop = loop;
}


