#ifndef HUSBANDO_CVLC_PLAYER_H_
#define HUSBANDO_CVLC_PLAYER_H_

#include "core/core.h"
#include "core/player.h"
#include <arc/std/bool.h>
#include <arc/std/time.h>

#ifdef __cplusplus
extern "C" {
#endif

//TODO: make this compatable with windows
#define HUSBANDO_CVLC_COMMAND_START       "cvlc -q "
#define HUSBANDO_CVLC_COMMAND_END         " 2> /dev/null & disown"

#define HUSBANDO_CVLC_SSH_COMMAND_START "systemd-run --user --scope --unit=cvlc-session --setenv=DISPLAY=:0 nohup setsid cvlc -q "
#define HUSBANDO_CVLC_SSH_COMMAND_END   " > /dev/null 2>&1 &"

/*
 * @brief controls
*/
#define HUSBANDO_CVLC_PLAYER_SEEK_RIGHT "playerctl position 5+"
#define HUSBANDO_CVLC_PLAYER_SEEK_LEFT  "playerctl position 5-"
#define HUSBANDO_CVLC_PLAYER_PLAY       "playerctl play"
#define HUSBANDO_CVLC_PLAYER_PAUSE      "playerctl pause"
#define HUSBANDO_CVLC_PLAYER_PLAY_PAUSE "playerctl play-pause"

/**
 * @breif creates the core of husbando, will init a player as well as a ssh connect (if the config defaults to connect), also loads in notes
 *
 * @param core     the core which will be created
 * @param playerId which video player to initialize
*/
void HUSBANDO_CorePlayer_CreateCVLCPlayer(HUSBANDO_CorePlayer **player);

/**
 * @breif cleans up the mpv player stored in the core player
 *
 * @param player the core player to destroy
*/
void HUSBANDO_CorePlayer_DestroyCVLCPlayer(HUSBANDO_CorePlayer *player);

/**
 * @breif inits a video in a player from a given url
 *
 * @note overrides HUSBANDO_CorePlayer_InitFn
 *
 * @param core     the arc_core which contains ssh data and a few other things that will be used with the init
 * @param url      the url or path that holds the video
 * @param autoPlay starts playing the video right when init finishes, if ARC_False the video will start paused
*/
void HUSBANDO_CVLC_InitFn(HUSBANDO_Core *core, ARC_String *url, ARC_Bool autoPlay);

/**
 * @breif will start playing a video if paused, also will do nothing if the video doesn't exist or is already playing
 *
 * @note overrides HUSBANDO_CorePlayer_PlayFn
 *
 * @param core the type holding the videoData that will be used
*/
void HUSBANDO_CVLC_PlayFn(HUSBANDO_Core *core);

/**
 * @breif will pause a playing video, also will do nothing if the video doesn't exist or is already paused
 *
 * @note overrides HUSBANDO_CorePlayer_PauseFn
 *
 * @param core the type holding the videoData that will be used
*/
void HUSBANDO_CVLC_PauseFn(HUSBANDO_Core *core);

/**
 * @breif seeks the current video player right
 *
 * @note overrides HUSBANDO_CorePlayer_SeekRightFn
 *
 * @param core the core that holds the video player
*/
void HUSBANDO_CVLC_SeekRightFn(HUSBANDO_Core *core);

/**
 * @breif seeks the current video player left
 *
 * @note overrides HUSBANDO_CorePlayer_SeekLeftFn
 *
 * @param core the core that holds the video player
*/
void HUSBANDO_CVLC_SeekLeftFn(HUSBANDO_Core *core);

/**
 * @breif gets the current time of the vide being played
 *
 * @note overrides HUSBANDO_CorePlayer_GetCurrentTimeFn
 *
 * @param core the core that holds the video player and video
 *
 * @return the current video time ARC_Time, or an ARC_Time struct filled with 0s on error
*/
ARC_Time HUSBANDO_CVLC_GetCurrentTimeFn(HUSBANDO_Core *core);

/**
 * @breif gets the full time of the vide being played
 *
 * @note overrides HUSBANDO_CorePlayer_GetFullTimeFn
 *
 * @param core the core that holds the video player and video
 *
 * @return the full video time ARC_Time, or an ARC_Time struct filled with 0s on error
*/
ARC_Time HUSBANDO_CVLC_GetFullTimeFn(HUSBANDO_Core *core);

/**
 * @breif sets the current time of the video being played
 *
 * @note overrides HUSBANDO_CorePlayer_SetCurrentTimeFn
 *
 * @param core the core that holds the video player and video
 * @param time the time to set the current playing video to
*/
void HUSBANDO_CVLC_SetCurrentTimeFn(HUSBANDO_Core *core, ARC_Time time);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_CVLC_PLAYER_H_
