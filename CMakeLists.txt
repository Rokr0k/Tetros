cmake_minimum_required (VERSION 3.8)

project ("tetros")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

find_package (SDL2 REQUIRED)
find_package (SDL2_image REQUIRED)
find_package (SDL2_mixer REQUIRED)
find_package (SDL2_ttf REQUIRED)

set (C_FILES "src/main.c" "src/game.c" "src/titleScene.c" "src/playScene.c" "src/tetris.c" "src/random.c" "src/font.c" "src/game.h" "src/titleScene.h" "src/playScene.h" "src/tetris.h" "src/random.h" "src/font.h" "src/gamecontroller.h" "src/gamecontroller.c" )
set (RESOURCES "res/title.png" "res/cursor.png" "res/tile.png" "res/tetrominos.png" "res/holdunav.png" "res/backgroundmusic.ogg" "res/AtariClassic.ttf" "res/gamecontrollerdb.txt")

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_executable (${PROJECT_NAME} ${C_FILES})
else ()
	add_executable (${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${C_FILES})
endif ()

target_link_libraries (${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::Mixer SDL2::TTF)

file (COPY ${RESOURCES} DESTINATION "${CMAKE_BINARY_DIR}/res")
