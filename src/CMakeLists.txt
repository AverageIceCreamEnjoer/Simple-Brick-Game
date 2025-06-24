cmake_minimum_required(VERSION 3.6.0)

project(Desk VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

#set(SOURCES tetris.a snake.a)
#add_library(lib STATIC ${SOURCES})
add_executable(Desk 
  start_game/start_desk.cc
#  brick_game/snake/snake.cc
#  brick_game/tetris/tetrisModel.cc
  controller/desk_controller.cc
  gui/desktop/desk.cc)

set(TETRIS_LIB_PATH "${CMAKE_SOURCE_DIR}/tetris.a")

set(SNAKE_LIB_PATH "${CMAKE_SOURCE_DIR}/snake.a")
target_link_libraries(Desk Qt6::Widgets ${TETRIS_LIB_PATH} ${SNAKE_LIB_PATH})