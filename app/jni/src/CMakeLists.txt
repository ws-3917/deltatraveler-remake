cmake_minimum_required(VERSION 3.10)
set(GAME_NAME RestTrylley)
project(${GAME_NAME} VERSION 1.0)

find_package(SDL3 CONFIG COMPONENTS SDL3)

add_library(main SHARED main.cpp)
target_link_libraries(main PRIVATE SDL3::SDL3)