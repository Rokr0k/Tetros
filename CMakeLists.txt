cmake_minimum_required (VERSION 3.8)

project ("Tetros")

find_package (SDL2 REQUIRED)
find_package (SDL2_IMAGE REQUIRED)
find_package (SDL2_MIXER REQUIRED)
find_package (SDL2_TTF REQUIRED)

include_directories (${SDL2_INCLUDE_DIRS})
include_directories (${SDL2_IMAGE_INCLUDE_DIRS})
include_directories (${SDL2_MIXER_INCLUDE_DIRS})
include_directories (${SDL2_TTF_INCLUDE_DIRS})

set (C_FILES "src/main.c" "src/game.c" "src/titleScene.c" "src/playScene.c" "src/tetris.c" "src/random.c" "src/font.c" "src/game.h" "src/titleScene.h" "src/playScene.h" "src/tetris.h" "src/random.h" "src/font.h")
set (RESOURCES "res/title.png" "res/tile.png" "res/tetrominos.png" "res/holdunav.png" "res/backgroundmusic.ogg" "res/ComicSans.ttf" "res/gamecontrollerdb.txt")

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_executable (Tetros ${C_FILES})
else ()
	add_executable (Tetros WIN32 MACOSX_BUNDLE ${C_FILES})
endif ()

file (COPY ${RESOURCES} DESTINATION "${CMAKE_BINARY_DIR}/res")

target_link_libraries (Tetros ${SDL2_LIBRARIES})
target_link_libraries (Tetros ${SDL2_IMAGE_LIBRARIES})
target_link_libraries (Tetros ${SDL2_MIXER_LIBRARIES})
target_link_libraries (Tetros ${SDL2_TTF_LIBRARIES})