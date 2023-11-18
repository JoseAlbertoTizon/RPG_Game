cmake_minimum_required(VERSION 3.26)
project(RPG)

set(CMAKE_CXX_STANDARD 17)

add_executable(RPG game.hpp
        main.cpp
        game.cpp
        object.cpp)

# SFML Linking
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/Project 3/SFML/include/SFML)
target_link_libraries(RPG sfml-system sfml-window sfml-graphics sfml-audio)
