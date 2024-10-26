cmake_minimum_required(VERSION 3.10)
set(GAME_NAME DeltaRemake)
project(${GAME_NAME} VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

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

# 安装可执行文件到根目录
# install(TARGETS ${GAME_NAME}
# RUNTIME DESTINATION bin
# )

# 安装 DLL 到根目录
# install(FILES
# $<TARGET_FILE:SDL3::SDL3>
# $<TARGET_FILE:SDL3_image::SDL3_image>
# $<TARGET_FILE:SDL3_mixer::SDL3_mixer>
# $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
# DESTINATION bin
# )

# 安装资源文件夹到根目录
# install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION .)

# 下面这些主要用于Windows MinGW构建的游戏运行
if(WIN32)
    # 获取 CXX 编译器的路径
    get_filename_component(MINGW_DLL_PATH "${CMAKE_CXX_COMPILER}" DIRECTORY)

    # 安装 MinGW 运行所需的 DLL 文件到 bin 目录
    install(FILES
        "${MINGW_DLL_PATH}/libgcc_s_seh-1.dll"
        "${MINGW_DLL_PATH}/libstdc++-6.dll"
        "${MINGW_DLL_PATH}/libwinpthread-1.dll"
        DESTINATION bin
    )
endif()