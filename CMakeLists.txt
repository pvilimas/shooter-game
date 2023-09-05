cmake_minimum_required(VERSION 3.15)
project(shooter-game)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(RAYLIB REQUIRED raylib)

add_executable(${PROJECT_NAME}
    src/assets.c
    src/callbacks.c
    src/game.c
    src/main.c
    src/scene.c
    src/utils.c
)

# glib
target_include_directories(${PROJECT_NAME} PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GLIB_LIBRARIES})

# raylib
target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${RAYLIB_LIBRARIES})

# math.h
target_link_libraries(${PROJECT_NAME} PRIVATE m)