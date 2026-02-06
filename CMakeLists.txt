cmake_minimum_required(VERSION 3.10)
project(s3t)

# This ensures Neovim's LSP can find your headers
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find the SDL3 library
find_package(SDL3 REQUIRED)

add_executable(main_app main.cpp)

# Link SDL3 to your app
target_link_libraries(main_app SDL3::SDL3)
