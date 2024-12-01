# Use host compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Compiler options for testing
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=c++11")

# Add Unity for testing
add_subdirectory(${CMAKE_SOURCE_DIR}/Unity ${CMAKE_BINARY_DIR}/Unity)

# Test source
set(TEST_SOURCE
    ${CMAKE_SOURCE_DIR}/tests.c
    ${CMAKE_SOURCE_DIR}/libcat.c
)

# Create test executable
add_executable(cat_tests ${TEST_SOURCE})

# Define macros for testing
target_compile_definitions(cat_tests PRIVATE TESTS)

# Add include directories
target_include_directories(cat_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/Unity/src
)

# Link with Unity
target_link_libraries(cat_tests PRIVATE unity)

# Enable testing
enable_testing()

# Add test
add_test(NAME cat_tests COMMAND cat_tests)
