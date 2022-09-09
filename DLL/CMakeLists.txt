cmake_minimum_required(VERSION 3.23)


set (CMAKE_C_COMPILER_WORKS 1)
set (CMAKE_CXX_COMPILER_WORKS 1)

project(SSD)

#set(SOURCE_FILES main)
#set(SOURCE_FILES NeuralLibrary)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

file(GLOB SOURCE_FILES

    "${PROJECT_SOURCE_DIR}/src/IOData/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/NeuralController/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/NeuralModules/*.cpp"
    "NeuralLibrary.cpp"
   #"main.cpp"
)
add_subdirectory("src/CUDA")
#add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}  CUDA_test)
include_directories("includes")
include_directories("lib")