#ifndef HUSBANDO_CORE_H_
#define HUSBANDO_CORE_H_

#include "player.h"
#include <stdint.h>
#include <arc/networking/ssh.h>
#include <arc/std/bool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HUSBANDO_CORE_VIDEO_PLAYER_NONE 0
#define HUSBANDO_CORE_VIDEO_PLAYER_MPV  1
#define HUSBANDO_CORE_VIDEO_PLAYER_CVLC 2

/**
 * @breif the core of husbando, holds ssh, video player, queue, and note
 *
 * @note TODO: add the queue and note stuff
 * @note TODO: handle notifications
*/
typedef struct HUSBANDO_Core {
    ARC_Ssh *ssh;

    ARC_String *videoName;
    ARC_String *videoUrl;
    ARC_Bool    videoAutoNext;
    uint64_t    videoLoop;

    HUSBANDO_CorePlayer *player;
    uint8_t              playerId;
} HUSBANDO_Core;

/**
 * @breif a global instance of core, will be set to NULL and need to be created somewhere
*/
extern HUSBANDO_Core *husbando_core;

/**
 * @breif creates the core of husbando, will init a player as well as a ssh connect (if the config defaults to connect), also loads in notes
 *
 * @param core     the core which will be created
 * @param playerId which video player to initialize
*/
void HUSBANDO_Core_Create(HUSBANDO_Core **core, uint8_t playerId);

/**
 * @breif cleans up the core of husbando, will save before destroying the data type
 *
 * @param core the core to destroy
*/
void HUSBANDO_Core_Destroy(HUSBANDO_Core *core);

/**
 * @breif sets the player to use in the core
 *
 * @param core     the core which will have its player set
 * @param playerId the video player to initialize and set into the core
*/
void HUSBANDO_Core_SetPlayer(HUSBANDO_Core *core, uint8_t playerId);

/**
 * @breif will create an ssh session when sshConnect is set to true or destroy a session if set to false
 *
 * @param core       the core to manage the ssh session of
 * @param sshConnect the bool that will either create or destroy an ssh session
*/
void HUSBANDO_Core_SetSsh(HUSBANDO_Core *core, ARC_Bool sshConnect);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_CORE_CONTROLS_H_
