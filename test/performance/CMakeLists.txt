cmake_minimum_required(VERSION 3.8)

set(THIS STL_PERFORMANCE_TESTS)

file(GLOB SOURCES "*.cpp")

add_executable(${THIS} ${SOURCES})

target_link_libraries(${THIS} PUBLIC
    gtest_main
    STL
)

add_test(
    NAME ${THIS}
    COMMAND ${THIS}
)