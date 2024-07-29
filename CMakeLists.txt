cmake_minimum_required(VERSION 3.14.0)

project(husbando LANGUAGES C VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(CMAKE_C_FLAGS "-Wall -Werror -g -ggdb -DARC_DEBUG ")

#TODO: add "-Wpedantic"
add_compile_options(
  "-Wall" "-fexceptions"
  "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
  "$<$<CONFIG:RELEASE>:-Wextra>"
)

set(ARCHEUS_STD_CONSOLE_BACKEND "NCURSES" CACHE STRING "use ncurses")
set(ARCHEUS_STD_SSH_BACKEND     "LIBSSH"  CACHE STRING "use libssh" )

find_package(json-c CONFIG REQUIRED)
add_subdirectory(lib/arc)

add_executable(husbando
    src/config.c
    src/main.c

    src/core/controls.c
    src/core/core.c
    src/core/episode.c

    src/provider/allanime/provider.c

    src/tui/base.c
    src/tui/container.c
    src/tui/page.c
    src/tui/pages/index.c
    src/tui/pages/notes.c
    src/tui/pages/search.c

    src/video/cvlc/player.c
    src/video/mpv/player.c

    #inih stuff
    lib/inih/ini.c
)

target_compile_definitions(husbando PRIVATE
    $<$<CONFIG:Debug>:ARC_DEBUG>
)

target_include_directories(husbando
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    lib/inih
)

target_link_libraries(husbando PUBLIC archeus_std curl json-c::json-c)
