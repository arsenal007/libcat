cmake_minimum_required(VERSION 3.10...3.26)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ================================================
# define: some Path lists to find executables
# 1) clang-format.exe for generating parameters
# 2) GNU Compilers
# 3) make
# 4) htool
# ================================================
list(APPEND CLANG_PATH_LIST "C:/msys64/mingw64/bin")
list(APPEND GCC_PATH_LIST
    "C:/Users/vkuzmenko/xPack/xpack-arm-none-eabi-gcc-13.2.1-1.1/bin"
    "C:/Users/vkuzmenko/xtensa-esp-elf/bin"
)

# find_program(MAKE_ABSOLUTE_PATH "mingw32-make.exe" ${MAKE_PATH_LIST})

# set(CMAKE_MAKE_PROGRAM ${MAKE_ABSOLUTE_PATH} CACHE FILEPATH "" FORCE)
if(TESTS)
    message(STATUS "Configuring for TESTS")
    include(${CMAKE_SOURCE_DIR}/cmake/host-testing.cmake)
elseif(ARM_CORTEX_M7)
    message(STATUS "Configuring for Cortex-M7")

    if(USE_CLANG)
        set(CMAKE_TOOLCHAIN_FILE cmake/cortex-m7-clang.cmake)
    else()
        set(CMAKE_TOOLCHAIN_FILE cmake/cortex-m7.cmake)
    endif()
elseif(ARM_CORTEX_M33)
    message(STATUS "Configuring for Cortex-M33")

    if(USE_CLANG)
        set(CMAKE_TOOLCHAIN_FILE cmake/cortex-m33-clang.cmake)
    else()
        set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/cortex-m33.cmake)
    endif()
elseif(XTENSA_LX6)
    message(STATUS "Configuring for Xtensa LX6")
    set(CMAKE_TOOLCHAIN_FILE cmake/xtensa-lx6.cmake)
elseif(RISCV_HAZARD3)
    message(STATUS "Configuring for RISC-V Hazard3")

    if(USE_CLANG)
        set(CMAKE_TOOLCHAIN_FILE cmake/riscv-hazard3-clang.cmake)
    else()
        set(CMAKE_TOOLCHAIN_FILE cmake/riscv-hazard3.cmake)
    endif()
else()
    message(FATAL_ERROR "No target specified. Please define ARM_CORTEX_M7, ARM_CORTEX_M33, XTENSA_LX6, or RISCV_HAZARD3.")
endif()

project(LIBCAT C CXX ASM)

if(NOT TESTS)
    # set(CMAKE_C_OUTPUT_EXTENSION .o)
    # set(CMAKE_CXX_OUTPUT_EXTENSION .o)
    set(CMAKE_VERBOSE_MAKEFILE on)

    # set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_BUILD_TYPE Release)

    find_program(CLANG_ABSOLUTE_PATH "clang-format.exe" ${CLANG_PATH_LIST})

    list(APPEND LIBCAT_SOURCE
        ${CMAKE_SOURCE_DIR}/libcat.c
    )

    # message(STATUS "LIBCAT_SOURCE contains: ${LIBCAT_SOURCE}")

    # Add include directories
    include_directories(${CMAKE_SOURCE_DIR}/Inc)

    add_library(cat STATIC ${LIBCAT_SOURCE})

    # target_include_directories(my_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )
    # target_compile_options(my_library PRIVATE -Wall -Wextra)
endif()