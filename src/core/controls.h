#ifndef HUSBANDO_CORE_CONTROLS_H_
#define HUSBANDO_CORE_CONTROLS_H_

#include "core.h"
#include <stdint.h>
#include <arc/networking/ssh.h>
#include <arc/std/bool.h>
#include <arc/std/time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @breif inits a video in a player from a given url, or notifies if the player doesn't exist
 *
 * @param core     the arc_core which contains ssh data and a few other things that will be used with the init
 * @param url      the url or path that holds the video
 * @param autoPlay starts playing the video right when init finishes, if ARC_False the video will start paused
*/
void HUSBANDO_Core_ControlsInitFn(HUSBANDO_Core *core, ARC_String *url, ARC_Bool autoPlay);

/**
 * @breif will start playing a video if paused, or notifies if the player doesn't exist or there is no playing video
 *
 * @param core the type holding the videoData that will be used
*/
void HUSBANDO_Core_ControlsPlayFn(HUSBANDO_Core *core);

/**
 * @breif will pause a playing video, or notifies if the player doesn't exist or there is no playing video
 *
 * @param core the type holding the videoData that will be used
*/
void HUSBANDO_Core_ControlsPauseFn(HUSBANDO_Core *core);

/**
 * @breif seeks the current video player right, or notifies if the player doesn't exist
 *
 * @param core the core that holds the video player
*/
void HUSBANDO_Core_ControlsSeekRight(HUSBANDO_Core *core);

/**
 * @breif seeks the current video player left, or notifies if the player doesn't exist
 *
 * @param core the core that holds the video player
*/
void HUSBANDO_Core_ControlsSeekLeft(HUSBANDO_Core *core);

/**
 * @breif gets the current time of the vide being played, or notifies if the player doesn't exist or there is no playing video
 *
 * @param core the core that holds the video player and video
 *
 * @return the current video time ARC_Time, or an ARC_Time struct filled with 0s on error
*/
ARC_Time HUSBANDO_Core_ControlsGetCurrentTime(HUSBANDO_Core *core);

/**
 * @breif gets the full time of the vide being played, or notifies if the player doesn't exist or there is no playing video
 *
 * @param core the core that holds the video player and video
 *
 * @return the full video time ARC_Time, or an ARC_Time struct filled with 0s on error
*/
ARC_Time HUSBANDO_Core_ControlsGetFullTime(HUSBANDO_Core *core);

/**
 * @breif sets the current time of the video being played, or notifies if the player doesn't exist or there is no playing video
 *
 * @param core the core that holds the video player and video
 * @param time the time to set the current playing video to
*/
void HUSBANDO_Core_ControlsSetCurrentTime(HUSBANDO_Core *core, ARC_Time time);

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
