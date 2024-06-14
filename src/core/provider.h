#ifndef HUSBANDO_CORE_PROVIDER_H_
#define HUSBANDO_CORE_PROVIDER_H_

#include <stdint.h>
#include <arc/std/string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct HUSBANDO_CoreProviderShow {
    ARC_String *name;
    uint32_t subEpisodesCount;
    uint32_t dubEpisodesCount;

    void *providerData;
} HUSBANDO_CoreProviderShow;

typedef struct HUSBANDO_CoreProviderEpisode {
} HUSBANDO_CoreProviderEpisode;

/**
 * @breif
 *
 * @param
*/
typedef void (* HUSBANDO_CoreProvider_SearchFn)(ARC_String *name);

/**
 * @breif
 *
 * @param
*/
typedef void (* HUSBANDO_CoreProvider_GetEpisodeFn)(ARC_String *name, uint32_t episode);

/**
 * @breif a struct that holds all of the functions needed to for a core provider
*/
typedef struct HUSBANDO_CoreProvider {
    HUSBANDO_CoreProvider_SearchFn     searchFn;
    HUSBANDO_CoreProvider_GetEpisodeFn getEpisodeFn;

    void *data;
} HUSBANDO_CoreProvider;

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_CORE_CONTROLS_H_
