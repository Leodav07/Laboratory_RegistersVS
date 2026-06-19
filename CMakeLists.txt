cmake_minimum_required(VERSION 3.16)
project(rtv_LAB)

set(CMAKE_CXX_STANDARD 20)

add_executable(rtv_LAB 
src/main.cpp 
src/CliArgs.cpp 
src/FileManager.cpp
src/PageManager.cpp)

target_include_directories(rtv_LAB PRIVATE include/)
