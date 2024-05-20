cmake_minimum_required(VERSION 3.14.0)

project(husbando LANGUAGES C VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS "-Wall -Werror -g -ggdb -DARC_DEBUG ")

set(ARCHEUS_STD_CONSOLE_BACKEND "NCURSES" CACHE STRING "use ncurses")

add_subdirectory(lib/arc)

find_package(LIBSSH REQUIRED)

add_executable(husbando
    src/config.c
    src/main.c

    src/core/controls.c
    src/core/core.c

    src/tui/base.c
    src/tui/container.c
    src/tui/page.c
    src/tui/pages/index.c
    src/tui/pages/notes.c

    src/video/mpv/player.c
)

target_include_directories(husbando
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(husbando PUBLIC archeus_std inih)
