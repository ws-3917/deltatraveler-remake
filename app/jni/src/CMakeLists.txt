cmake_minimum_required(VERSION 3.10)
set(GAME_NAME RestTrylley)
project(${GAME_NAME} VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
# 找到所有 SDL3 相关库
if (NOT TARGET SDL3::SDL3)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
endif()

if (NOT TARGET SDL3_image::SDL3_image)
find_package(SDL3_image REQUIRED CONFIG REQUIRED COMPONENTS SDL3_image)
endif()

if (NOT TARGET SDL3_mixer::SDL3_mixer)
find_package(SDL3_mixer REQUIRED CONFIG REQUIRED COMPONENTS SDL3_mixer)
endif()

if (NOT TARGET SDL3_ttf::SDL3_ttf)
find_package(SDL3_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL3_ttf)
endif()

# 添加可执行文件
add_library(main SHARED code/main.cpp)

# 统一链接 SDL3 库
target_link_libraries(main PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf)