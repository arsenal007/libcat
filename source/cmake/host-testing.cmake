# Use host compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Compiler options for testing
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=c++11")

# Add Unity for testing
add_subdirectory(${CMAKE_SOURCE_DIR}/Unity ${CMAKE_BINARY_DIR}/Unity)

# Create test executable
add_executable(cat_tests)

target_sources(cat_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/libcat.c
    ${CMAKE_SOURCE_DIR}/tests/tests.c
    ${CMAKE_SOURCE_DIR}/tests/fa_tests.c
    ${CMAKE_SOURCE_DIR}/tests/cat_decode_received_cmd_tests.c
    ${CMAKE_SOURCE_DIR}/tests/set_tests.c
    ${CMAKE_SOURCE_DIR}/tests/if_tests.c
    ${CMAKE_SOURCE_DIR}/tests/id_tests.c
    ${CMAKE_SOURCE_DIR}/tests/md_tests.c
    ${CMAKE_SOURCE_DIR}/tests/ks_tests.c
)

# Define macros for testing
target_compile_definitions(cat_tests PRIVATE TESTS)

# Add include directories
target_include_directories(cat_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Unity/src
)

# Link with Unity
target_link_libraries(cat_tests PRIVATE unity)

# Enable testing
enable_testing()

# Add test
add_test(NAME cat_tests COMMAND cat_tests)
