cmake_minimum_required(VERSION 3.14)
project(GAME VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/libs)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC OFF)

set(GAME_SOURCES
    main.cpp

    classes/cell.cpp
    classes/field.cpp

    classes/player.cpp
    classes/playerHand.cpp

    classes/ally.cpp
    classes/allyManager.cpp

    classes/enemy.cpp
    classes/enemyManager.cpp
    classes/enemyBuilding.cpp
    classes/enemyTower.cpp

    classes/areaDamageSpell.cpp
    classes/directDamageSpell.cpp
    classes/trapSpell.cpp
    classes/allySpell.cpp
    classes/improveSpell.cpp

    classes/gameManager.cpp
    classes/saveManager.cpp
    classes/game.cpp

    gui/gameGUI.cpp
    gui/textureManager.cpp
    gui/enemyInfoWindow.cpp
)

add_executable(game ${GAME_SOURCES})

target_include_directories(game PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/gui_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
)

target_link_libraries(game sfml-graphics sfml-window sfml-system)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/resources)
    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:game>/resources
    )
endif()