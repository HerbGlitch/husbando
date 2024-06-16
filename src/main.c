/* testing allanime */
#include "core/provider.h"
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
    ARC_String_CreateWithStrlen(&name, "tonikawa");

    ARC_String *language;
    ARC_String_CreateWithStrlen(&language, "dub");

    ARC_Vector *shows = provider->searchFn(provider, name, language);
    HUSBANDO_CoreProviderEpisode *episode = provider->getEpisodeFn(provider, (HUSBANDO_CoreProviderShow *)ARC_Vector_Get(shows, 0), 1);

    for(uint32_t i = 0; i < ARC_Vector_Size(shows); i++){
        HUSBANDO_CoreProviderShow *show = (HUSBANDO_CoreProviderShow *)ARC_Vector_Get(shows, i);
        printf("%s\n", show->name->data);
        ARC_String_Destroy(show->name);
        ARC_String_Destroy(show->language);
        ARC_String_Destroy(show->providerId);
        free(show);
    }
    ARC_Vector_Destroy(shows);

    ARC_String_Destroy(name);
    ARC_String_Destroy(language);

    HUSBANDO_CoreProvider_DestroyAllanimeProvider(provider);

    printf("url: %s\n", episode->url->data);
    ARC_String_Destroy(episode->url);
    free(episode);
//    temp1();

//    ARC_String *tempCurlResponse;
//    HUSBANDO_Allanime_GetCurlResponse(&tempCurlResponse, "https://allanime.day/apivtwo/clock.json?id=7d2473746a243c246e727276753c2929636b6472676d73287674692975727463676b6f686128766e76396f623b4b6c43714942473720726f726a633b44696d732d68692d4d696d6974692d68692d5f6764676f2d5f677275732d23343e42736423343f2d43766f756962632d3720727f76637573643b555344242a2475727463676b63744f62243c24706f62636975727463676b6f69242a2462677263243c24343634322b36302b373452363f3c373e3c3636283636365c247b&referer=");
//    printf("\n\n%s\n\n", tempCurlResponse->data);

//    ARC_String *name;
//    ARC_String_CreateWithStrlen(&name, "Kaguya-sama wa Kokurasetai: Tensai-tachi no Renai Zunousen");

//    HUSBANDO_Allanime_GetEpisodeURLString(name, 1);

//    HUSBANDO_Allanime_Search(name);

//    ARC_String_Destroy(name);
    return 0;
}

/* end testing */

/* testing tui */
/*
#include "config.h"
#include "core/core.h"
#include "tui/container.h"
#include "tui/page.h"
#include "tui/pages/index.h"
#include <arc/console/view.h>
#include <arc/math/rectangle.h>

int main(int argc, char *argv[]){
    HUSBANDO_Config_Init("res/config/husbando.ini");

    HUSBANDO_Core_Create(&husbando_core, HUSBANDO_CORE_VIDEO_PLAYER_MPV);

    HUSBANDO_TUIContainer *container;
    HUSBANDO_TUIContainer_Create(&container, (char *)husbando_config.Tui.containerName, NULL, 100);

    ARC_Rect pageSize = ARC_ConsoleView_GetBounds(container->view);

    HUSBANDO_TUIPage *page;
    HUSBANDO_TUIPage_CreateIndex(&page, container, pageSize);

    HUSBANDO_TUIContainer_SetPage(container, page);
    HUSBANDO_TUIContainer_RunPage(container);

    HUSBANDO_TUIPage_DestroyIndex(page);

    HUSBANDO_TUIContainer_Destory(container);

    HUSBANDO_Core_Destroy(husbando_core);

    HUSBANDO_Config_Deinit();
    return 0;
}
*/
/* end testing*/
