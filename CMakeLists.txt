cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 11)

project(TinyRenderer)

add_executable(${PROJECT_NAME} src/main.cpp src/lib/tgaimage.cpp src/lib/model.cpp)
