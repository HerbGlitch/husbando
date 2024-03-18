#include <curses.h>
#include <libssh/libssh.h>
//#include "arc/ssh/ssh.h"
#include "arc/math/point.h"
#include "arc/ncurses/ncurses.h"
#include "arc/ncurses/element.h"
#include "arc/std/string.h"

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

int main(int argc, char *argv[]){
    // ARC_Ssh_Create(&ssh);
    ARC_NCurses *mainNCurses;
    ARC_NCurses_Create(&mainNCurses, NULL);
    ARC_NCurses_SetBorder(mainNCurses, ARC_NCURSES_BORDER_DEFAULT);

    ARC_String *string0;
    ARC_String *string1;
    ARC_String *string2;
    ARC_String *string3;

    ARC_String_CreateWithStrlen(&string0, "OPTION 1");
    ARC_String_CreateWithStrlen(&string1, "OPTION 2");
    ARC_String_CreateWithStrlen(&string2, "OPTION 3");
    ARC_String_CreateWithStrlen(&string3, "OPTION 4");

    ARC_NCursesElement *element0;
    ARC_NCursesElement *element1;
    ARC_NCursesElement *element2;
    ARC_NCursesElement *element3;

    ARC_NCursesElement_Create(&element0, 0, ARC_NCURSES_ELEMENT_FLAG_NONE, string0, (ARC_Point){1, 1}, ARC_NCursesElement_DefaultRenderFn);
    ARC_NCursesElement_Create(&element1, 0, ARC_NCURSES_ELEMENT_FLAG_NONE, string1, (ARC_Point){1, 2}, ARC_NCursesElement_DefaultRenderFn);
    ARC_NCursesElement_Create(&element2, 0, ARC_NCURSES_ELEMENT_FLAG_NONE, string2, (ARC_Point){1, 3}, ARC_NCursesElement_DefaultRenderFn);
    ARC_NCursesElement_Create(&element3, 0, ARC_NCURSES_ELEMENT_FLAG_NONE, string3, (ARC_Point){1, 4}, ARC_NCursesElement_DefaultRenderFn);

    ARC_NCurses_AddElement(mainNCurses, element0);
    ARC_NCurses_AddElement(mainNCurses, element1);
    ARC_NCurses_AddElement(mainNCurses, element2);
    ARC_NCurses_AddElement(mainNCurses, element3);

    ARC_NCursesElement *selectedElement = ARC_NCurses_GetElement(mainNCurses, 0);
    ARC_NCursesElement_SetSelected(selectedElement, ARC_True);

    ARC_NCurses_RenderElements(mainNCurses);

    int32_t character = getch();
    int32_t selected = 0;
    while(character != KEY_F(1)){
        ARC_NCursesElement_SetSelected(selectedElement, ARC_False);

        switch(character){
            case KEY_UP:
                if(selected == 0){
                    break;
                }
                selected--;
                break;

            case KEY_DOWN:
                if(selected == 3){
                    break;
                }
                selected++;
                break;
        }

        selectedElement = ARC_NCurses_GetElement(mainNCurses, selected);
        ARC_NCursesElement_SetSelected(selectedElement, ARC_True);

        ARC_NCurses_RenderElements(mainNCurses);
        character = getch();
    }

    ARC_NCurses_RemoveElement(mainNCurses, 0);
    ARC_NCurses_RemoveElement(mainNCurses, 0);
    ARC_NCurses_RemoveElement(mainNCurses, 0);
    ARC_NCurses_RemoveElement(mainNCurses, 0);

    ARC_String_Destroy(element0->string);
    ARC_String_Destroy(element1->string);
    ARC_String_Destroy(element2->string);
    ARC_String_Destroy(element3->string);

    ARC_NCursesElement_Destroy(element0);
    ARC_NCursesElement_Destroy(element1);
    ARC_NCursesElement_Destroy(element2);
    ARC_NCursesElement_Destroy(element3);

    ARC_NCurses_Destroy(mainNCurses);

    // ARC_Ssh_Destroy(ssh);
    return 0;
}
