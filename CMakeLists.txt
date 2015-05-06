cmake_minimum_required(VERSION 3.1)

project(vtop)

find_package(Curses REQUIRED)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)


add_executable(vtop ${SOURCE_FILES} main.cpp process.h process.cpp)
target_link_libraries(vtop ${CURSES_LIBRARIES})

include_directories(${CURSES_INCLUDE_DIR})


