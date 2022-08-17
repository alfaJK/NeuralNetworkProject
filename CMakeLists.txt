cmake_minimum_required(VERSION 3.23)


set (CMAKE_C_COMPILER_WORKS 1)
set (CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER "C:/gcc/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/gcc/bin/g++.exe")

project(SSD)

set(SOURCE_FILES main)

file(GLOB SOURCE_FILES

    "${PROJECT_SOURCE_DIR}/src/IOData/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/NeuralController/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/NeuralModules/*.cpp"
    "*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories("includes")
include_directories("lib")