cmake_minimum_required(VERSION 3.10)

project(GAME)

# SDL sources are in a subfolder named "SDL"
add_subdirectory(SDL)
add_subdirectory(SDL_image)
add_subdirectory(SDL_mixer)
add_subdirectory(SDL_ttf)
# add_subdirectory(SDL_net)

# Compilation of companion libraries

# Your game and its CMakeLists.txt are in a subfolder named "src"
add_subdirectory(src)

