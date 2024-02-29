cmake_minimum_required(VERSION 3.14.0)

project(ani-gui-pi LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS "-Wall -Werror -g -ggdb -DARC_DEBUG ")

add_subdirectory(lib/arc)

find_package(LIBSSH REQUIRED)

add_executable(ani-gui-pi
    src/main.c

    src/arc/ncurses/ncurses.c

    src/arc/ssh/ssh.c
)

target_include_directories(ani-gui-pi
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(ani-gui-pi PUBLIC archeus_std ssh ncurses)
