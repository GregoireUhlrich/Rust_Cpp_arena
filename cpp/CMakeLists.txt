cmake_minimum_required(VERSION "3.5")

project(cpp-benchmarks CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_compile_definitions(DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
add_compile_definitions(RESULTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/results")

add_subdirectory(src)

add_executable(main main.cpp)
target_link_libraries(main tools-obj benchmark-obj)
