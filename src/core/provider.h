#ifndef HUSBANDO_CORE_PROVIDER_H_
#define HUSBANDO_CORE_PROVIDER_H_

#include "show.h"
#include <stdint.h>
#include <arc/std/bool.h>
#include <arc/std/string.h>
#include <arc/std/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @predefine the HUSBANOD_CoreProvider type to use in the callbacks
*/
typedef struct HUSBANDO_CoreProvider HUSBANDO_CoreProvider;

/**
 * @breif searches a provider for a show matching a name (partial or full)
 *
 * @note each HUSBANDO_CoreProviderShow in the returned vector and the returned vector iself needs to be freed
 * @note allowing adult content will be pulled from the main config
 *
 * @param provider   a reference to the provider to allow storing of data
 * @param name       the name of the show to search for
 * @param language   the language (Exmple: sub, dub) to get the show in, Look at the provider for what options this parameter can take
 *
 * @return a vector of HUSBANOD_CoreProviderShow types on success, or NULL on failure
*/
typedef ARC_Vector *(* HUSBANDO_CoreProvider_SearchFn)(HUSBANDO_CoreProvider *provider, ARC_String *name, ARC_String *language);

/**
 * @breif gets the details and url of a show at an episode
 *
 * @note returned value needs to be freed if it is not NULL
 *
 * @param provider a reference to the provider to allow storing of data
 * @param show     the show to get the episode from
 * @param episode  the episode to get the details of
 *
 * @return the episode and its details on success, or NULL on failure
*/
typedef HUSBANDO_CoreProviderEpisode *(* HUSBANDO_CoreProvider_GetEpisodeFn)(HUSBANDO_CoreProvider *provider, HUSBANDO_CoreProviderShow *show, uint32_t episode);

/**
 * @breif gets the details and url of a show at an episode
 *
 * @note returned value needs to be freed if it is not NULL
 *
 * @param provider a reference to the provider to allow storing of data
 * @param showId   the show id to get the episode from
 * @param episode  the episode to get the details of
 *
 * @return the episode and its details on success, or NULL on failure
*/
typedef HUSBANDO_CoreProviderEpisode *(* HUSBANDO_CoreProvider_GetEpisodeByShowIdFn)(HUSBANDO_CoreProvider *provider, ARC_String *showId, uint32_t episode);

/**
 * @breif gets a url from an episode of a specific resolution
 *
 * @note returned value needs to be freed if it is not NULL
 *
 * @param provider   a reference to the provider to allow storing of data
 * @param episode    the episode to get the resolution from
 * @param resolution the desired resolution to select
 *
 * @return a url on success, or NULL on failure
*/
typedef ARC_String *(* HUSBANDO_CoreProvider_GetResolutionUrlFn)(HUSBANDO_CoreProvider *provider, HUSBANDO_CoreProviderEpisode *episode, ARC_String *resolution);

/**
 * @breif a struct that holds all of the functions needed to for a core provider
*/
struct HUSBANDO_CoreProvider {
    HUSBANDO_CoreProvider_SearchFn             searchFn;
    HUSBANDO_CoreProvider_GetEpisodeFn         getEpisodeFn;
    HUSBANDO_CoreProvider_GetEpisodeByShowIdFn getEpisodeByShowIdFn;
    HUSBANDO_CoreProvider_GetResolutionUrlFn   getResolutionUrlFn;

    void *data;
};

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_CORE_PROVIDER_H_
