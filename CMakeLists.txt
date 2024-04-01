cmake_minimum_required(VERSION 3.27)
project(SO2_Projekt)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "-lncurses")

add_executable(SO2_Projekt main.cpp
        objects/Ball.cpp
        objects/Ball.h
        objects/Rectangle.cpp
        objects/Rectangle.h
        utility/Render.cpp
        utility/Render.h
)
