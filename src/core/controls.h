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
 * @breif
 *
 * @param
*/
void HUSBANDO_Core_ControlsVolumeIncrement(HUSBANDO_Core *core);

/**
 * @breif
 *
 * @param
*/
void HUSBANDO_Core_ControlsVolumeDecrement(HUSBANDO_Core *core);

/**
 * @breif
 *
 * @param
*/
void HUSBANDO_Core_ControlsSeekLeft(HUSBANDO_Core *core);

/**
 * @breif
 *
 * @param
*/
void HUSBANDO_Core_ControlsSeekRight(HUSBANDO_Core *core);

/**
 * @breif
 *
 * @param
*/
void HUSBANDO_Core_ControlsSetTime(HUSBANDO_Core *core, struct tm *timeinfo);

/**
 * @breif
 *
 * @param
*/
struct tm *HUSBANDO_Core_ControlsGetTime(HUSBANDO_Core *core);

/**
 * @breif
 *
 * @param
*/
void HUSBANDO_Core_ControlsSetAutoNext(HUSBANDO_Core *core, ARC_Bool autoNext);

/**
 * @brief
 *
 * @param
*/
void HUSBANDO_Core_ControlsSetLoop(HUSBANDO_Core *core, uint64_t loop);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_CORE_CONTROLS_H_
