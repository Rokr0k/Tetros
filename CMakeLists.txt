cmake_minimum_required (VERSION 3.8)

project ("tetros")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

set (C_SOURCES "src/main.c" "src/game.c" "src/titleScene.c" "src/playScene.c" "src/tetris.c" "src/random.c" "src/font.c" "src/gamecontroller.c" )
set (C_HEADERS "src/game.h" "src/titleScene.h" "src/playScene.h" "src/tetris.h" "src/random.h" "src/font.h" "src/gamecontroller.h")
set (RESOURCES "res/title.png" "res/tile.png" "res/tetrominos.png" "res/holdunav.png" "res/backgroundmusic.ogg" "res/AtariClassic.ttf" "res/gamecontrollerdb.txt")

add_executable (${PROJECT_NAME} ${C_SOURCES} ${C_HEADERS})

target_include_directories (${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries (${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES})

file (COPY ${RESOURCES} DESTINATION "${CMAKE_BINARY_DIR}/res")
