cmake_minimum_required(VERSION 3.13.4)

project(DuneII)

# set up SDL2pp library
SET(SDL2PP_WITH_IMAGE YES)
SET(SDL2PP_WITH_TTF YES)
SET(SDL2PP_WITH_MIXER YES)
add_subdirectory(external/SDL2pp)

# add compilation flags
SET(GCC_COVERAGE_LINK_FLAGS "-lSDL2_mixer")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -g0 -ggdb")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
ADD_DEFINITIONS(-DDATA_PATH="${PROJECT_SOURCE_DIR}/data")
INCLUDE_DIRECTORIES("${SDL2PP_INCLUDE_DIRS}")

# set up yaml-cpp library
add_subdirectory(external/yaml-cpp)

# set up QT library
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Multimedia REQUIRED)

ADD_EXECUTABLE("server" server_main.cpp
            src/common_protocol.cpp
            src/common_resolver.cpp
            src/common_socket.cpp
            src/common_utils.cpp
            src/server_clienthandler.cpp
            src/server_gameresources.cpp
            src/server_astar.cpp
            src/server_board.cpp
            src/server_building.cpp
            src/server_buildingfactory.cpp
            src/server_cell.cpp
            src/server_selectable.cpp
            src/server_server.cpp
            src/server_terrain.cpp
            src/server_tsq.cpp
            src/server_unit.cpp
            src/server_unitfactory.cpp)

ADD_EXECUTABLE("client" client_main.cpp
            src/common_protocol.cpp
            src/common_socket.cpp
            src/common_resolver.cpp
            src/common_utils.cpp
            src/client_player.cpp
            src/client_audioplayer.cpp
            src/client_selectable.cpp
            src/client_printer.cpp
            src/client_gamemap.cpp
            src/client_renderhandler.cpp
            src/client_gamehud.cpp
            src/client_gamehud_button.cpp
            src/client_camera.cpp
            src/client_mouse.cpp
            src/client_shot.cpp
            src/client_shotshandler.cpp
            src/ui/vistas.h
            src/ui/mainwindow.cpp
            src/ui/mainwindow.ui
            src/ui/lobbywindow.cpp
            src/ui/lobbywindow.ui
            src/ui/factionwindow.cpp
            src/ui/factionwindow.ui
            src/ui/creditswindow.cpp
            src/ui/creditswindow.ui
            src/ui/resources.qrc)

target_include_directories("server" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)  
target_include_directories("client" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)            


TARGET_LINK_LIBRARIES("server" "${SDL2PP_LIBRARIES}" Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia yaml-cpp)
TARGET_LINK_LIBRARIES("client" "${SDL2PP_LIBRARIES}" Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia yaml-cpp)

