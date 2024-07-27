#include "episode.h"

#include <stdlib.h>
#include <arc/std/string.h>
#include <arc/std/vector.h>

void HUSBANDO_CoreProviderEpisode_CreateEmpty(HUSBANDO_CoreProviderEpisode **episode){
    *episode = (HUSBANDO_CoreProviderEpisode *)malloc(sizeof(HUSBANDO_CoreProviderEpisode));

    (*episode)->title = NULL;

    ARC_Vector_Create(&((*episode)->urls));

    (*episode)->currentUrl = 0;
    (*episode)->currentEpisode = 0;

    (*episode)->providerData = NULL;
}

void HUSBANDO_CoreProviderEpisode_Destroy(HUSBANDO_CoreProviderEpisode *episode){
    if(episode->title != NULL){
        free(episode->title);
    }

    for(uint32_t index = 0; index < ARC_Vector_Size(episode->urls); index++){
        ARC_String *url = (ARC_String *)ARC_Vector_Get(episode->urls, index);
        ARC_String_Destroy(url);
    }
    ARC_Vector_Destroy(episode->urls);

    free(episode);
}
