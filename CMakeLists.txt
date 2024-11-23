cmake_minimum_required(VERSION 3.29)
project(snake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(snake src/main.cpp
        src/controller/Game.cpp
        src/controller/Game.h
        src/screen/Screen.cpp
        src/screen/Screen.h
        src/entity/Snake.cpp
        src/entity/Snake.h
        src/controller/Coordinate.cpp
        src/controller/Coordinate.h
        src/controller/Direction.h
        src/entity/Cell.cpp
        src/entity/Cell.h
        src/screen/ScreenType.h
        src/controller/GameStatus.h
)
target_link_libraries(snake raylib)


#for linux - raylib is already installed correctly (see in README.md how to correctly install raylib for Linux)

find_package(raylib REQUIRED)


#for windows - raylib is already installed

#include_directories(C:/raylib/w64devkit/x86_64-w64-mingw32/include)
#link_directories(C:/raylib/w64devkit/x86_64-w64-mingw32/lib)
#
#if (WIN32)
#    target_link_libraries(snake gdi32 winmm)
#endif ()


#for any OS with no raylib installed manually (but if you have Linux, you need to have all dependencies for raylib being installed)

#include(FetchContent)
#set(FETCHCONTENT_QUIET FALSE)
#set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
#set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
#
#FetchContent_Declare(
#        raylib
#        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
#        GIT_TAG "master"
#        GIT_PROGRESS TRUE
#)
#
#FetchContent_MakeAvailable(raylib)
#
## Adding our source files
#file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/sources/*.c") # Define PROJECT_SOURCES as a list of all source files
#set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/sources/") # Define PROJECT_INCLUDE to be the path to the include directory of the project
#
## Declaring our executable
#target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
#target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
