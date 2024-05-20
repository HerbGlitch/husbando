#include "config.h"

#include <ini.h>
#include <stdlib.h>
#include <string.h>

HUSBANDO_Config husbando_config = {
    { NULL, NULL, NULL }, //Main
    { 0 }, //Local
    { 0, NULL, NULL, NULL }, //Ssh
    { 0, 0, 1920, 1080 }, //Gui
    { NULL }, //Tui
    { NULL } //Mpv
};

void HUSBANDO_Config_Init(const char *path){
    if(ini_parse(path, HUSBANDO_Config_HandlerFn, &husbando_config) < 0) {
        //TODO: notify then create a default ini file
    }
}

void HUSBANDO_Config_Deinit(){
    //clean up main
    if(husbando_config.Main.configPath != NULL){
        free((void *)husbando_config.Main.configPath);
    }

    if(husbando_config.Main.filePath != NULL){
        free((void *)husbando_config.Main.filePath);
    }

    if(husbando_config.Main.videoPath != NULL){
        free((void *)husbando_config.Main.videoPath);
    }

    //ssh
    if(husbando_config.Ssh.hostname != NULL){
        free((void *)husbando_config.Ssh.hostname);
    }

    if(husbando_config.Ssh.user != NULL){
        free((void *)husbando_config.Ssh.user);
    }

    if(husbando_config.Ssh.password != NULL){
        free((void *)husbando_config.Ssh.password);
    }

    //clean up tui
    if(husbando_config.Tui.containerName != NULL){
        free((void *)husbando_config.Tui.containerName);
    }

    //clean up mpv
    if(husbando_config.Mpv.socketName != NULL){
        free((void *)husbando_config.Mpv.socketName);
    }
}

int32_t HUSBANDO_Config_HandlerFn(void *user, const char *section, const char *name, const char *value){
    HUSBANDO_Config *config = (HUSBANDO_Config *)user;

    //main
    if(strcmp(section, "main") == 0 && strcmp(name, "configPath") == 0){
        config->Main.configPath = strdup(value);
        return 1;
    }

    if(strcmp(section, "main") == 0 && strcmp(name, "filePath") == 0){
        config->Main.filePath = strdup(value);
        return 1;
    }

    if(strcmp(section, "main") == 0 && strcmp(name, "videoPath") == 0){
        config->Main.videoPath = strdup(value);
        return 1;
    }

    //local
    if(strcmp(section, "local") == 0 && strcmp(name, "player") == 0){
        config->Local.player = atoi(value);
        return 1;
    }

    //ssh
    if(strcmp(section, "ssh") == 0 && strcmp(name, "player") == 0){
        config->Ssh.player = atoi(value);
        return 1;
    }

    if(strcmp(section, "ssh") == 0 && strcmp(name, "hostname") == 0){
        config->Ssh.hostname = strdup(value);
        return 1;
    }

    if(strcmp(section, "ssh") == 0 && strcmp(name, "user") == 0){
        config->Ssh.user = strdup(value);
        return 1;
    }

    if(strcmp(section, "ssh") == 0 && strcmp(name, "password") == 0){
        config->Ssh.password = strdup(value);
        return 1;
    }

    //gui
    if(strcmp(section, "gui") == 0 && strcmp(name, "windowX") == 0){
        config->Gui.windowX = atoi(value);
        return 1;
    }

    if(strcmp(section, "gui") == 0 && strcmp(name, "windowY") == 0){
        config->Gui.windowY = atoi(value);
        return 1;
    }

    if(strcmp(section, "gui") == 0 && strcmp(name, "windowW") == 0){
        config->Gui.windowW = atoi(value);
        return 1;
    }

    if(strcmp(section, "gui") == 0 && strcmp(name, "windowH") == 0){
        config->Gui.windowH = atoi(value);
        return 1;
    }

    //tui
    if(strcmp(section, "tui") == 0 && strcmp(name, "containerName") == 0){
        config->Tui.containerName = strdup(value);
        return 1;
    }

    //mpv
    if(strcmp(section, "mpv") == 0 && strcmp(name, "socketName") == 0){
        config->Mpv.socketName = strdup(value);
        return 1;
    }

    //unknown, so return 0
    return 0;
}
