/* testing allanime */
#include "provider/allanime/provider.h"
#include <arc/std/string.h>

int main(){
    temp();
    temp1();

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