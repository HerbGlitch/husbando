#ifndef HUSBANDO_CORE_H_
#define HUSBANDO_CORE_H_

#include <stdint.h>
#include <arc/networking/ssh.h>
#include <arc/std/bool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HUSBANDO_CORE_VIDEO_PLAYER_MPV  0
#define HUSBANDO_CORE_VIDEO_PLAYER_CVLC 1

/**
 * @breif
*/
typedef struct HUSBANDO_Core {
    ARC_Ssh *ssh;

    ARC_String *videoName;
    ARC_String *videoUrl;
    uint8_t videoPlayer;

    void *videoData;
} HUSBANDO_Core;

/**
 * @breif
 *
 * @param
*/
void HUSBANDO_Core_Create(HUSBANDO_Core **core, uint8_t videoPlayer);

/**
 * @breif
 *
 * @param
*/
void HUSBANDO_Core_Destroy(HUSBANDO_Core *core);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_CORE_CONTROLS_H_
