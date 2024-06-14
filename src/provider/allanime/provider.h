#ifndef HUSBANDO_ALLANIME_PROVIDER_H_
#define HUSBANDO_ALLANIME_PROVIDER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <arc/std/string.h>
/*
 * @NOTE: this file has a fair ammount of temprorary things
*/

/*
 * @brief provider details
 *
 * @note this info can be found in https://github.com/pystardust/ani-cli
*/
#define HUSBANDO_ALLANIME_REFERER   "https://allanime.to"
#define HUSBANDO_ALLANIME_BASE      "allanime.day"
#define HUSBANDO_ALLANIME_API       "https://api." HUSBANDO_ALLANIME_BASE "/api"
#define HUSBANDO_ALLANIME_USERAGENT "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/121.0"

/*
 * @brief data urlencode defines
 *
 * @note this info can be found in https://github.com/pystardust/ani-cli
*/
#define HUSBANDO_ALLANIME_SEARCH_QUERY           "query(        $search: SearchInput        $limit: Int        $page: Int        $translationType: VaildTranslationTypeEnumType        $countryOrigin: VaildCountryOriginEnumType    ) {    shows(        search: $search        limit: $limit        page: $page        translationType: $translationType        countryOrigin: $countryOrigin    ) {        edges {            _id name availableEpisodes __typename       }    }}"
#define HUSBANDO_ALLANIME_SEARCH_VARIABLES_START "{\"search\":{\"allowAdult\":false,\"allowUnknown\":false,\"query\":\""
#define HUSBANDO_ALLANIME_SEARCH_VARIABLES_END   "\"},\"limit\":40,\"page\":1,\"translationType\":\"dub\",\"countryOrigin\":\"ALL\"}"

#define HUSBANDO_ALLANIME_EPISODE_QUERY "query ($showId: String!, $translationType: VaildTranslationTypeEnumType!, $episodeString: String!) {    episode(        showId: $showId        translationType: $translationType        episodeString: $episodeString    ) {        episodeString sourceUrls    }}"

typedef struct HUSBANDO_ProviderShow {
    ARC_String *name;
    uint32_t subEpisodesCount;
    uint32_t dubEpisodesCount;

    void *providerData;
} HUSBANDO_ProviderShow;

typedef struct HUSBANDO_ProiderEpisode {
}

typedef struct HUSBANDO_ProviderEpisode {

} HUSBANDO_ProviderEpisode;

//TODO: implement this
//void HUSBANDO_CoreProvider_CreateAllanimeProvider(HUSBANDO_CoreProvider **provider);

//TODO: implement this
//void HUSBANDO_CoreProvider_DestroyAllanimeProvider(HUSBANDO_CoreProvider *provider);

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
 * @breif searches all anime for a given show name
 *
 * @param name the name of the show to search
*/
void HUSBANDO_Allanime_Search(ARC_String *name);

/**
 * @breif searches all anime for a given show name
 *
 * @param name the name of the show to search
 *
 * @return a url on success, NULL on fail
*/
ARC_String *HUSBANDO_Allanime_GetEpisodeURL(ARC_String *name, uint32_t episode);

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

void temp();
void temp1();

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_ALLANIME_PROVIDER_H_
