#ifndef HUSBANDO_CORE_PLAYER_H_
#define HUSBANDO_CORE_PLAYER_H_

#include <stdint.h>
#include <time.h>
#include <arc/std/bool.h>
#include <arc/std/string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief predefine of husbando core to be use in the core players callbacks
*/
typedef struct HUSBANDO_Core HUSBANDO_Core;

/**
 * @breif inits a video in a player from a given url
 *
 * @param core     the arc_core which contains ssh data and a few other things that will be used with the init
 * @param url      the url or path that holds the video
 * @param autoPlay starts playing the video right when init finishes, if ARC_False the video will start paused
*/
typedef void (* HUSBANDO_CorePlayer_InitFn)(HUSBANDO_Core *core, ARC_String *url, ARC_Bool autoPlay);

/**
 * @breif will start playing a video if paused, also will do nothing if the video doesn't exist or is already playing
 *
 * @param core the type holding the videoData that will be used
*/
typedef void (* HUSBANDO_CorePlayer_PlayFn)(HUSBANDO_Core *core);

/**
 * @breif will pause a playing video, also will do nothing if the video doesn't exist or is already paused
 *
 * @param core the type holding the videoData that will be used
*/
typedef void (* HUSBANDO_CorePlayer_PauseFn)(HUSBANDO_Core *core);

/**
 * @breif will skip a video to the right a specified ammount of time stored in core, also will do nothing if the video doesn't exist
 *
 * @param core the type holding the videoData that will be used
*/
typedef void (* HUSBANDO_CorePlayer_SeekRightFn)(HUSBANDO_Core *core);

/**
 * @breif will skip a video to the left a specified ammount of time stored in core, also will do nothing if the video doesn't exist
 *
 * @param core the type holding the videoData that will be used
*/
typedef void (* HUSBANDO_CorePlayer_SeekLeftFn)(HUSBANDO_Core *core);

/**
 * @breif will set the video players current time to a specified time, also will do nothing if the video doesn't exist
 *
 * @param core     the type holding the videoData that will be used
 * @param timeinfo the time to set the video player to
*/
typedef void (* HUSBANDO_CorePlayer_SetCurrentTimeFn)(HUSBANDO_Core *core, struct tm *timeinfo);

/**
 * @breif will get the video's current time
 *
 * @param core the type holding the videoData that will be used
 *
 * @return the current time, or NULL if the video doesn't exist
*/
typedef struct tm *(* HUSBANDO_CorePlayer_GetCurrentTimeFn)(HUSBANDO_Core *core);

/**
 * @breif will get the video's full time
 *
 * @param core the type holding the videoData that will be used
 *
 * @return the full time of the playing video, or NULL if the video doesn't exist
*/
typedef struct tm *(* HUSBANDO_CorePlayer_GetFullTimeFn)(HUSBANDO_Core *core);

/**
 * @breif a struct that holds all of the functions needed to for a core player
*/
typedef struct HUSBANDO_CorePlayer {
    HUSBANDO_CorePlayer_InitFn           initFn;
    HUSBANDO_CorePlayer_PlayFn           playFn;
    HUSBANDO_CorePlayer_PauseFn          pauseFn;
    HUSBANDO_CorePlayer_SeekRightFn      seekRightFn;
    HUSBANDO_CorePlayer_SeekLeftFn       seekLeftFn;
    HUSBANDO_CorePlayer_SetCurrentTimeFn setCurrentTimeFn;
    HUSBANDO_CorePlayer_GetCurrentTimeFn getCurrentTimeFn;
    HUSBANDO_CorePlayer_GetFullTimeFn    getFullTimeFn;
} HUSBANDO_CorePlayer;

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_CORE_CONTROLS_H_
