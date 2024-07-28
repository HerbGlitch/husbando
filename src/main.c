/* testing allanime */
/*
//#include "core/provider.h"
#include "core/episode.h"
#include "core/show.h"
#include "provider/allanime/provider.h"
#include <stdio.h>
#include <stdlib.h>
#include <arc/std/string.h>
#include <arc/std/vector.h>

int main(int32_t argc, char **argv){
    HUSBANDO_CoreProvider *provider;
    HUSBANDO_CoreProvider_CreateAllanimeProvider(&provider);

    ARC_String *name;
    //ARC_String_CreateWithStrlen(&name, "d-frag");
    ARC_String_CreateWithStrlen(&name, "tanya");

    ARC_String *language;
    ARC_String_CreateWithStrlen(&language, "dub");

    ARC_Vector *shows = provider->searchFn(provider, name, language);
    HUSBANDO_CoreProviderEpisode *episode = provider->getEpisodeFn(provider, (HUSBANDO_CoreProviderShow *)ARC_Vector_Get(shows, 0), 1);

    for(uint32_t i = 0; i < ARC_Vector_Size(shows); i++){
        HUSBANDO_CoreProviderShow *show = (HUSBANDO_CoreProviderShow *)ARC_Vector_Get(shows, i);
        ARC_String_Destroy(show->name);
        ARC_String_Destroy(show->language);
        ARC_String_Destroy(show->providerId);
        free(show);
    }
    ARC_Vector_Destroy(shows);

    ARC_String_Destroy(name);
    ARC_String_Destroy(language);

    HUSBANDO_CoreProvider_DestroyAllanimeProvider(provider);

    if(ARC_Vector_Size(episode->urls) != 0){
        ARC_String *url = (ARC_String *)ARC_Vector_Get(episode->urls, 0);
        printf("url: %s\n", url->data);
    }
    else {
        printf("episodes are empty\n");
    }

    HUSBANDO_CoreProviderEpisode_Destroy(episode);

    return 0;
}
*/
/* end testing */

/* testing tui */
#include "config.h"
#include "core/core.h"
#include "tui/container.h"
#include "tui/page.h"
#include <arc/console/view.h>
#include <arc/math/rectangle.h>

int main(int argc, char *argv[]){
    HUSBANDO_Config_Init("res/config/husbando.ini");

    HUSBANDO_Core_Create(&husbando_core, HUSBANDO_CORE_VIDEO_PLAYER_MPV, HUSBANDO_CORE_VIDEO_PROVIDER_ALLANIME);

    HUSBANDO_TUIContainer *container;
    HUSBANDO_TUIContainer_Create(&container, (char *)husbando_config.Tui.containerName, NULL, 50);

    ARC_Rect pageSize = ARC_ConsoleView_GetBounds(container->view);

    HUSBANDO_TUIPage *page;
    HUSBANDO_TUIPage_CreateWithId(&page, HUSBANDO_TUI_PAGE_ID_INDEX, container, pageSize);

    HUSBANDO_TUIContainer_SetPage(container, page);
    HUSBANDO_TUIContainer_RunPage(container);

    page = HUSBANDO_TUIContainer_GetPage(container);
    if(page != NULL){
        HUSBANDO_TUIPage_Destroy(page);
    }

    HUSBANDO_TUIContainer_Destory(container);

    HUSBANDO_Core_Destroy(husbando_core);

    HUSBANDO_Config_Deinit();
    return 0;
}
/* end testing*/
