# CMake toolchain file for ESP32-C3
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)

# Compiler flags specific to ESP32-C3
set(MCPU_FLAGS "-march=rv32imc -mabi=ilp32 -mcpu=sifive-e31 -mtune=size")

# Common search paths for the toolchain
list(APPEND TOOLCHAIN_HINTS
    $ENV{HOME}/.espressif/tools/riscv32-esp-elf/*
    C:/Users/$ENV{USERNAME}/.espressif/tools/riscv32-esp-elf/*
    $ENV{HOME}/.platformio/packages/toolchain-riscv32-esp/*
    C:/Users/$ENV{USERNAME}/.platformio/packages/toolchain-riscv32-esp/*
)

# Locate riscv32-esp-elf-gcc
find_program(CMAKE_C_COMPILER
    NAMES riscv32-esp-elf-gcc
    HINTS ${TOOLCHAIN_HINTS}
    PATH_SUFFIXES bin
)

# Locate riscv32-esp-elf-g++ (C++ compiler)
find_program(CMAKE_CXX_COMPILER
    NAMES riscv32-esp-elf-g++
    HINTS ${TOOLCHAIN_HINTS}
    PATH_SUFFIXES bin
)

# Locate riscv32-esp-elf-ar (archiver)
find_program(AR
    NAMES riscv32-esp-elf-ar
    HINTS ${TOOLCHAIN_HINTS}
    PATH_SUFFIXES bin
)

# Locate riscv32-esp-elf-objcopy
find_program(CMAKE_OBJCOPY
    NAMES riscv32-esp-elf-objcopy
    HINTS ${TOOLCHAIN_HINTS}
    PATH_SUFFIXES bin
)

# Locate riscv32-esp-elf-objdump
find_program(CMAKE_OBJDUMP
    NAMES riscv32-esp-elf-objdump
    HINTS ${TOOLCHAIN_HINTS}
    PATH_SUFFIXES bin
)

# Default to error if tools are not found
if(NOT CMAKE_C_COMPILER OR NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "ESP32-C3 toolchain not found. Please ensure riscv32-esp-elf-gcc and related tools are installed.")
endif()

# Compiler flags
set(CMAKE_C_FLAGS "${MCPU_FLAGS} -Wall -Wextra -fno-builtin -ffunction-sections -fdata-sections -std=gnu11")
set(CMAKE_CXX_FLAGS "${MCPU_FLAGS} -Wall -Wextra -fno-builtin -ffunction-sections -fdata-sections -std=gnu++17 -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS "${MCPU_FLAGS} -x assembler-with-cpp")

# Debug and release configurations
set(CMAKE_C_FLAGS_DEBUG "-Og -g3")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g3")
set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG")
