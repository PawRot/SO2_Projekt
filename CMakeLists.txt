cmake_minimum_required(VERSION 3.27)
project(SO2_Projekt)

set(CMAKE_CXX_STANDARD 23)
if (LINUX)
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
endif ()
set(CMAKE_CXX_FLAGS "-lncurses")

add_executable(SO2_Projekt main.cpp
        objects/Ball.cpp
        objects/Ball.h
        objects/Rectangle.cpp
        objects/Rectangle.h
        utility/Render.cpp
        utility/Render.h
)

if (LINUX)
target_link_libraries(SO2_Projekt ${CURSES_LIBRARIES})
endif ()
