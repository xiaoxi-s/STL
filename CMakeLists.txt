cmake_minimum_required(VERSION 3.8)

set(THIS SSTL)

project(${THIS} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(googletest)

file(GLOB SOURCES "src/*.cpp")
file(GLOB SOURCES "src/*/*.cpp")
file(GLOB HEADERS "src/*.hpp")

add_library(${THIS} STATIC ${SOURCES} ${HEADERS})

set_target_properties(SSTL PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)