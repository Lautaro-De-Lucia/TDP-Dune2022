cmake_minimum_required(VERSION 3.13.4)

project(DuneII)

# set up SDL2pp library
SET(SDL2PP_WITH_IMAGE YES)
SET(SDL2PP_WITH_TTF YES)
add_subdirectory(external/SDL2pp)

# add compilation flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -g3")
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

ADD_EXECUTABLE("${PROJECT_NAME}" main.cpp
            src/astar.cpp
            src/board.cpp
            src/building.cpp
            src/buildingfactory.cpp
            src/cell.cpp
            src/player.cpp
            src/selectable.cpp
            src/terrain.cpp
            src/textfilehandler.cpp
            src/unit.cpp
            src/unitfactory.cpp
            src/client_player.cpp 
            src/client_selectable.cpp
            src/client_printer.cpp
            src/client_gamemap.cpp
            src/client_renderhandler.cpp
            src/client_gamehud.cpp
            src/client_gamehud_button.cpp
            src/common_utils.cpp
            src/client_camera.cpp
            src/mouse.cpp
            src/ui/vistas.h
            src/ui/mainwindow.cpp
            src/ui/mainwindow.ui
            src/ui/lobbywindow.cpp
            src/ui/lobbywindow.ui
            src/ui/resources.qrc)

target_include_directories("${PROJECT_NAME}" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)            

TARGET_LINK_LIBRARIES("${PROJECT_NAME}" "${SDL2PP_LIBRARIES}" Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia yaml-cpp)

