cmake_minimum_required(VERSION 3.10...3.26)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if(TESTS)
    message(STATUS "Configuring for TESTS")
    include(${CMAKE_SOURCE_DIR}/cmake/host-testing.cmake)
elseif(ARM_CORTEX_M7)
    message(STATUS "Configuring for Cortex-M7")

    if(USE_CLANG)
        set(CMAKE_TOOLCHAIN_FILE cmake/cortex-m7-clang.cmake)
    else()
        set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/cortex-m7.cmake)
    endif()
elseif(ARM_CORTEX_M33)
    message(STATUS "Configuring for Cortex-M33")

    if(USE_CLANG)
        set(CMAKE_TOOLCHAIN_FILE cmake/cortex-m33-clang.cmake)
    else()
        set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/cortex-m33.cmake)
    endif()
elseif(ARM_CORTEX_M0PLUS)
    message(STATUS "Configuring for Cortex-M0+")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/cortex-m0plus.cmake)
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
elseif(ESP32_C3)
    message(STATUS "Configuring for ESP32-C3")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/esp32-c3.cmake)
else()
    message(FATAL_ERROR "No target specified. Please define ARM_CORTEX_M7, ARM_CORTEX_M33, XTENSA_LX6, or RISCV_HAZARD3.")
endif()

project(LIBCAT C CXX ASM)

# ---- Only build if not tests ----
if(NOT TESTS)
    set(CMAKE_VERBOSE_MAKEFILE ON)
    set(CMAKE_BUILD_TYPE Release)

    add_library(cat STATIC)

    target_sources(cat
        PRIVATE
        ${CMAKE_SOURCE_DIR}/libcat.c
    )

    target_include_directories(cat
        PUBLIC
        ${CMAKE_SOURCE_DIR}/Inc
    )

    target_include_directories(cat
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    )

    install(TARGETS cat
        EXPORT catTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )

    install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
    )
endif()