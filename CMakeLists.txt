cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 11)

project(TinyRenderer)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/tgaimage.cpp
        src/model.cpp
        src/geometry.cpp
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/src
)