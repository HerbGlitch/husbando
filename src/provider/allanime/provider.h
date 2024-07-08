#ifndef HUSBANDO_ALLANIME_PROVIDER_H_
#define HUSBANDO_ALLANIME_PROVIDER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "core/provider.h"
#include <arc/std/string.h>
#include <arc/std/vector.h>

/*
 * @brief allanime provider details
 *
 * @note this info can be found in https://github.com/pystardust/ani-cli
*/
#define HUSBANDO_ALLANIME_REFERER   "https://allanime.to"
#define HUSBANDO_ALLANIME_BASE      "allanime.day"
#define HUSBANDO_ALLANIME_API       "https://api." HUSBANDO_ALLANIME_BASE "/api"
#define HUSBANDO_ALLANIME_USERAGENT "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/121.0"

/**
 * @breif creates a husbando core provider type filled with allanime functions
 *
 * @param provider the parameter to store the allanime provider in
*/
void HUSBANDO_CoreProvider_CreateAllanimeProvider(HUSBANDO_CoreProvider **provider);

/**
 * @breif destroys a husbando core provider type and cleans up allanime stored data
 *
 * @param provider the parameter to destroy the allanime provider in
*/
void HUSBANDO_CoreProvider_DestroyAllanimeProvider(HUSBANDO_CoreProvider *provider);

/**
 * @breif will try to get a curl repsonse to allanime from a given url
 *
 * @note this function uses the HUSBANDO_ALLANIME_ defines
 * @note responseString will be set to NULL and arc_errno will be set on fail
 * @note responseString will be created if no error happens, it is on you to clean it up
 *
 * @param responseString the string retrieved from the curl call
 * @param url            the url to run curl on
*/
void HUSBANDO_Allanime_GetCurlResponse(ARC_String **responseString, char *url);

/**
 * @breif searches a provider for a show matching a name (partial or full) with allanime
 *
 * @note this function matches HUSBANDO_CoreProivder_SearchFn
 * @note each HUSBANDO_CoreProviderShow in the returned vector and the returned vector iself needs to be freed
 * @note allowing adult content will be pulled from the main config
 *
 * @param provider   a reference to the provider to allow storing of data
 * @param name       the name of the show to search for
 * @param language   the language, either "sub" or "dub"
 *
 * @return a vector of HUSBANOD_CoreProviderShow types on success, or NULL on failure
*/
ARC_Vector *HUSBANDO_Allanime_Search(HUSBANDO_CoreProvider *provider, ARC_String *name, ARC_String *language);

/**
 * @breif gets the details and url of a show at an episode
 *
 * @note this function matches HUSBANDO_CoreProivder_GetEpisodeFn
 * @note returned value needs to be freed if it is not NULL
 *
 * @param provider      a reference to the provider to allow storing of data
 * @param show          the show to get the episode from
 * @param episodeNumber the episode to get the details of
 *
 * @return the episode and its details on success, or NULL on failure
*/
HUSBANDO_CoreProviderEpisode *HUSBANDO_Allanime_GetEpisode(HUSBANDO_CoreProvider *provider, HUSBANDO_CoreProviderShow *show, uint32_t episodeNumber);

/**
 * @breif searches all anime for a given show name
 *
 * @note the returned HUSBANDO_ProviderEpisode needs to be deleted
 *
 * @param name the name of the show to search
 *
 * @return an episode that holds a url and ids of different resolutions for the episode or NULL on fail
*/
void HUSBANDO_Allanime_GetCurrentEpisodeURL(ARC_String *resolution);

/**
 * @breif
 *
 * @param outputId
 * @param inputId
*/
void HUSBANDO_Allanime_SubstitueId(ARC_String **outputId, ARC_String *inputId);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_ALLANIME_PROVIDER_H_
