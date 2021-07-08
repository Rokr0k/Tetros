cmake_minimum_required (VERSION 3.8)

project ("Tetros")

find_package (SDL2 REQUIRED)
find_package (SDL2_image REQUIRED)
find_package (SDL2_mixer REQUIRED)
find_package (SDL2_ttf REQUIRED)
find_package (SDL2_net REQUIRED)

include_directories (${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_NET_INCLUDE_DIRS})

set (C_FILES "src/main.c" "src/game.c" "src/titleScene.c" "src/singlePlayScene.c" "src/tetris.c" "src/random.c" "src/font.c" "src/game.h" "src/titleScene.h" "src/singlePlayScene.h" "src/tetris.h" "src/random.h" "src/font.h" "src/multiConnectScene.h" "src/multiConnectScene.c" "src/gamecontroller.h" "src/gamecontroller.c" "src/multiServerScene.h" "src/multiServerScene.c" "src/multiClientScene.h" "src/multiClientScene.c" "src/multiPlayScene.h" "src/multiPlayScene.c" )
set (RESOURCES "res/title.png" "res/cursor.png" "res/tile.png" "res/tetrominos.png" "res/holdunav.png" "res/backgroundmusic.ogg" "res/AtariClassic.ttf" "res/gamecontrollerdb.txt")

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_executable (Tetros ${C_FILES})
else ()
	add_executable (Tetros WIN32 MACOSX_BUNDLE ${C_FILES})
endif ()

file (COPY ${RESOURCES} DESTINATION "${CMAKE_BINARY_DIR}/res")

target_link_libraries (Tetros ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_NET_LIBRARIES})