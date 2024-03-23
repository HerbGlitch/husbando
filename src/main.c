#include "arc/console/buffer.h"
#include "arc/std/bool.h"
#include <ncurses.h>
#include <arc/console/view.h>
#include <arc/console/shell.h>
#include <arc/console/element.h>
#include <arc/math/point.h>
#include <arc/math/rectangle.h>
#include <arc/std/string.h>
//#include "arc/ssh/ssh.h"

// static ARC_Ssh *ssh;

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

void Temp_ConsoleShell_UpdateFn(ARC_ConsoleShell *shell){
    ARC_String *prompt;
    ARC_String_CreateWithStrlen(&prompt, "SHELL > ");

    ARC_Point currentPos = (ARC_Point){ 1, 1 };

    ARC_Bool running = ARC_True;
    while(running){
        ARC_ConsoleView_RenderStringAt(shell->view, prompt, currentPos);

        ARC_String *input = ARC_ConsoleView_GetStringInput(shell->view, (ARC_Point){ currentPos.x + prompt->length, currentPos.y });
        currentPos.y++;
        if(input == NULL){
            continue;
        }

        if(ARC_String_SubstringEqualsCString(input, 0, "exit", 4)){
            running = ARC_False;
        }

        ARC_ConsoleBuffer_AddString(shell->buffer, input);
        ARC_String_Destroy(input);
        ARC_ConsoleBuffer_Render(shell->buffer, shell->view);
    }

    ARC_String_Destroy(prompt);
}

int main(int argc, char *argv[]){
    // ARC_Ssh_Create(&ssh);
    ARC_ConsoleView *view;
    ARC_ConsoleView_Create(&view, (ARC_Rect){ 0, 0, 0, 0 });
    //ARC_ConsoleView_SetBorder(view, ARC_CONSOLE_VIEW_BORDER_DEFAULT);

    ARC_ConsoleShell *shell;
    ARC_ConsoleShell_Create(&shell, view, Temp_ConsoleShell_UpdateFn);

    ARC_ConsoleShell_Render(shell);

    ARC_ConsoleShell_Update(shell);

    ARC_ConsoleShell_Destroy(shell);
    ARC_ConsoleView_Destroy(view);

    // ARC_Ssh_Destroy(ssh);
    return 0;
}
