#ifndef HUSBANDO_CORE_EPISODE_H_
#define HUSBANDO_CORE_EPISODE_H_

#include <stdint.h>
#include <arc/std/string.h>
#include <arc/std/time.h>
#include <arc/std/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @breif a struct that holds all of the data relating to an episode of a show
 *
 * @TODO: might want to have episode specific ratings and notes
 *
 * @note urls will store ARC_String types
*/
typedef struct HUSBANDO_CoreProviderEpisode {
    ARC_String *title;
    ARC_Vector *urls;
    uint32_t currentUrl;
    uint32_t currentEpisode;

    void *providerData;
} HUSBANDO_CoreProviderEpisode;

/**
 * @breif creates and empty core provider episode
 *
 * @note this will initialize the urls vector
 * @note title and providerData will be set to NULL
 *
 * @brief episode the episode to fill with empty and 0 values
*/
void HUSBANDO_CoreProviderEpisode_CreateEmpty(HUSBANDO_CoreProviderEpisode **episode);

/**
 * @breif destroys a given provider episode
 *
 * @note this will NOT free providerData, so use this function only if providerData is already freed
 *
 * @brief episode the episode to destroy
*/
void HUSBANDO_CoreProviderEpisode_Destroy(HUSBANDO_CoreProviderEpisode *episode);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_CORE_EPISODE_H_
