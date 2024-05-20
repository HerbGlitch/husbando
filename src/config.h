#ifndef HUSBANDO_CONFIG_H_
#define HUSBANDO_CONFIG_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief the main config type (husbando.ini should match this)
*/
typedef struct HUSBANDO_Config {
    struct {
        const char *configPath;
        const char *filePath;
        const char *videoPath;
    } Main;

    struct {
        uint32_t player;
    } Local;

    struct {
        uint32_t player;
        const char *hostname;
        const char *user;
        const char *password;
    } Ssh;

    struct {
        int32_t windowX;
        int32_t windowY;
        int32_t windowW;
        int32_t windowH;
    } Gui;

    struct {
        const char *containerName;
    } Tui;

    struct {
        const char *socketName;
    } Mpv;
} HUSBANDO_Config;

/**
 * @brief a global config variable
*/
extern HUSBANDO_Config husbando_config;

/**
 * @brief reads in an ini file into the HUSBANDO_Config type
 *
 * @note this function will set husbando_config
 *
 * @param path the path of the ini file to read in
*/
void HUSBANDO_Config_Init(const char *path);

/**
 * @brief deinits husbando_config
 *
 * @note this function will clean up the memory of husbando_config
*/
void HUSBANDO_Config_Deinit();

/**
 * @brief
 *
 * @note overrides the hander function callback for ini.h
 *
 * @param user
 * @param section
 * @param name
 * @param value
*/
int32_t HUSBANDO_Config_HandlerFn(void *user, const char *section, const char *name, const char *value);

#ifdef __cplusplus
}
#endif

#endif // !HUSBANDO_CONFIG_H_
