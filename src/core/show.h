#ifndef HUSBANDO_CORE_SHOW_H_
#define HUSBANDO_CORE_SHOW_H_

#include <stdint.h>
#include <arc/std/string.h>
#include <arc/std/time.h>
#include <arc/std/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @breif a struct that holds all of the data relating to a show
 *
 * @note providerData should only be used by the current provider
 *
 * @TODO: have option to store provider ids and other data in saved file
*/
typedef struct HUSBANDO_CoreProviderShow {
    ARC_String *name;
    ARC_String *language;
    uint32_t subWatchedEpisodes;
    uint32_t dubWatchedEpisodes;
    uint32_t subTotalEpisodes;
    uint32_t dubTotalEpisodes;

    ARC_Time lastWatchedDate;
    ARC_Time startedWatchingDate;
    ARC_Time firstWathedDate;
    uint32_t numberOfRewatches;

    ARC_String *resolution;

    ARC_Vector *ratings;
    ARC_Vector *tags;
    ARC_String *notes;

    ARC_String *providerId;
    void       *providerData;
} HUSBANDO_CoreProviderShow;

/**
 * @breif a struct that holds all of the data relating to an episode of a show
 *
 * @TODO: might want to have episode specific ratings and notes
*/
typedef struct HUSBANDO_CoreProviderEpisode {
    ARC_String *title;
    ARC_String *url;
    uint32_t currentEpisode;

    void *providerData;
} HUSBANDO_CoreProviderEpisode;

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_CORE_SHOW_H_
