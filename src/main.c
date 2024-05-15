#include "tui/container.h"
#include "tui/page.h"
#include "tui/pages/index.h"
#include <arc/console/view.h>
#include <arc/math/rectangle.h>

int main(int argc, char *argv[]){
    HUSBANDO_TUIContainer *container;
    HUSBANDO_TUIContainer_Create(&container, "Husbando", NULL, 100);

    ARC_Rect pageSize = ARC_ConsoleView_GetBounds(container->view);

    HUSBANDO_TUIPage *page;
    HUSBANDO_TUIPage_CreateIndex(&page, container, pageSize);

    HUSBANDO_TUIContainer_SetPage(container, page);
    HUSBANDO_TUIContainer_RunPage(container);

    HUSBANDO_TUIPage_DestroyIndex(page);

    HUSBANDO_TUIContainer_Destory(container);
    return 0;
}

//TODO: will remove this when it is implemented in the core
// ARC_Ssh_ExecStrInNewSession(ssh, "export DISPLAY=:0 ; volume --inc");
// ARC_Ssh_ExecStrInNewSession(ssh, "export DISPLAY=:0 ; volume --dec");
// ARC_Ssh_ExecStrInNewSession(ssh, "sudo ydotool key 105:1 105:0");
// ARC_Ssh_ExecStrInNewSession(ssh, "sudo ydotool key 106:1 106:0");
// ARC_Ssh_ExecStrInNewSession(ssh, "sudo ydotool key 57:1 57:0");

/*
alias pspace='ydotool key 57:1 57:0'
alias pright='ydotool key 106:1 106:0'
alias pleft='ydotool key 105:1 105:0'
alias pdesktop1='ydotool key 125:1 2:1 2:0 125:0'
alias pdesktop2='ydotool key 125:1 3:1 3:0 125:0'
alias pdesktop3='ydotool key 125:1 4:1 4:0 125:0'
alias pdesktop4='ydotool key 125:1 5:1 5:0 125:0'
alias pclose='ydotool key 125:1 46:1 46:0 125:0'
*/


