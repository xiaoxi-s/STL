cmake_minimum_required(VERSION 3.8)

add_subdirectory("correctness")
# add_subdirectory("performance")

# set(THIS STL_TESTS)

# file(GLOB SOURCES "*.cpp")

# add_executable(${THIS} ${SOURCES})

# target_link_libraries(${THIS} PUBLIC
#     gtest_main
#     STL
# )

# add_test(
#     NAME ${THIS}
#     COMMAND ${THIS}
# )