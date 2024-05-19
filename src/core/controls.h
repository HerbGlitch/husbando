#ifndef HUSBANDO_CORE_CONTROLS_H_
#define HUSBANDO_CORE_CONTROLS_H_

#include "core.h"
#include <stdint.h>
#include <time.h>
#include <arc/networking/ssh.h>
#include <arc/std/bool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @breif seeks the current video player left, or notifies if the player doesn't exist
 *
 * @param core the core that holds the video player
*/
void HUSBANDO_Core_ControlsSeekLeft(HUSBANDO_Core *core);

/**
 * @breif seeks the current video player right, or notifies if the player doesn't exist
 *
 * @param core the core that holds the video player
*/
void HUSBANDO_Core_ControlsSeekRight(HUSBANDO_Core *core);

/**
 * @breif sets the current time of the video being played, or notifies if the player doesn't exist or there is no playing video
 *
 * @note TODO: change struct tm to an ARC_Time
 *
 * @param core     the core that holds the video player and video
 * @param timeinfo the time to set the current playing video to
*/
void HUSBANDO_Core_ControlsSetCurrentTime(HUSBANDO_Core *core, struct tm *timeinfo);

/**
 * @breif gets the current time of the vide being played, or notifies if the player doesn't exist or there is no playing video
 *
 * @note do not free the returned tm struct
 * @note TODO: change struct tm to an ARC_Time
 *
 * @param core the core that holds the video player and video
 *
 * @return the current video time as a tm struct or NULL
*/
struct tm *HUSBANDO_Core_ControlsGetCurrentTime(HUSBANDO_Core *core);

/**
 * @breif gets the full time of the vide being played, or notifies if the player doesn't exist or there is no playing video
 *
 * @note do not free the returned tm struct
 * @note TODO: change struct tm to an ARC_Time
 *
 * @param core the core that holds the video player and video
 *
 * @return the full video time as a tm struct or NULL
*/
struct tm *HUSBANDO_Core_ControlsGetFullTime(HUSBANDO_Core *core);

/**
 * @breif sets if auto play should be turned on or off, or notifies if the player doesn't exist
 *
 * @param core     the core that holds the video player
 * @param autoNext a bool for if the player should auto play the next video
*/
void HUSBANDO_Core_ControlsSetAutoNext(HUSBANDO_Core *core, ARC_Bool autoNext);

/**
 * @brief sets a value for if the video should loop
 *
 * @note if the value passed is ~(uint64_t)0 then the loop will loop ifinitely
 *
 * @param core the core that holds the video player
 * @param the number of times to loop, 0 means the video will not loop
*/
void HUSBANDO_Core_ControlsSetLoop(HUSBANDO_Core *core, uint64_t loop);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_CORE_CONTROLS_H_
