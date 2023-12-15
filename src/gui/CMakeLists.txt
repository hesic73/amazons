cmake_minimum_required(VERSION 3.16.0)

project(GUI)

set(CMAKE_INCLUDE_CURRENT_DIR ON) 

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")


find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(gui 
    main.cpp 
    mainwindow.hpp
)

target_link_libraries(gui 
    Qt5::Widgets
)