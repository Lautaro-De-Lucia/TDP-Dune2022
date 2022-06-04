cmake_minimum_required(VERSION 3.13.4)

project(DuneII)
# set up SDL2pp library
SET(SDL2PP_WITH_IMAGE YES)
SET(SDL2PP_WITH_TTF YES)
add_subdirectory(external/SDL2pp)

# add compilation flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
ADD_DEFINITIONS(-DDATA_PATH="${PROJECT_SOURCE_DIR}/data")
INCLUDE_DIRECTORIES("${SDL2PP_INCLUDE_DIRS}")

# ADD_EXECUTABLE("${PROJECT_NAME}" main.cpp)

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
            src/unitfactory.cpp)

target_include_directories("${PROJECT_NAME}" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)            

TARGET_LINK_LIBRARIES("${PROJECT_NAME}" "${SDL2PP_LIBRARIES}")


#target_include_directories(${PROJECT_NAME}
#	PUBLIC external/SDL2pp
#)

#target_link_directories(${PROJECT_NAME}
#	PRIVATE external/SDL2pp
#)

#target_link_libraries(${PROJECT_NAME} SDL2pp)

