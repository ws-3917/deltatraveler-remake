cmake_minimum_required(VERSION 3.10)
set(GAME_NAME DeltaRemake)
project(${GAME_NAME} VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)

# 找到所有 SDL3 相关库
if(NOT TARGET SDL3::SDL3)
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
    find_package(SDL3_image REQUIRED CONFIG REQUIRED COMPONENTS SDL3_image-shared)
    find_package(SDL3_mixer REQUIRED CONFIG REQUIRED COMPONENTS SDL3_mixer-shared)
    find_package(SDL3_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL3_ttf-shared)
endif()

# 添加可执行文件
add_executable(${GAME_NAME} code/main.cpp apps/icon.rc) # 增加控制台输出

# 统一链接 SDL3 库
target_link_libraries(${GAME_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf)