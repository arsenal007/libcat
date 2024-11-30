# Set the system name and processor type for RISC-V Hazard3
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)

# Define RISC-V-specific compiler flags
# Hazard3 may only support "rv32im" without FPU and uses "ilp32" ABI
set(MCPU_FLAGS "-march=rv32im -mabi=ilp32 -mtune=generic")
set(EXTRA_FLAGS "-ffunction-sections -fdata-sections -Wall -Wextra -fno-builtin")

# Toolchain settings
set(CMAKE_C_COMPILER    riscv32-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER  riscv32-unknown-elf-g++)
set(AS                  riscv32-unknown-elf-as)
set(AR                  riscv32-unknown-elf-ar)

# Locate GCC tools (optional if paths are correctly set in the environment)
find_program(GCC_PATH_ABSOLUTE ${CMAKE_C_COMPILER} ${GCC_PATH_LIST} NO_DEFAULT_PATH)
get_filename_component(ADD_TO_PATH_GCC ${GCC_PATH_ABSOLUTE} PATH) 
list(APPEND CMAKE_PROGRAM_PATH ${ADD_TO_PATH_GCC})

# Compiler and assembler flags
set(CMAKE_C_FLAGS   "${MCPU_FLAGS} ${EXTRA_FLAGS} -std=gnu11" CACHE INTERNAL "C compiler flags")
set(CMAKE_CXX_FLAGS "${MCPU_FLAGS} ${EXTRA_FLAGS} -std=gnu++17 -fno-exceptions -fno-rtti" CACHE INTERNAL "C++ compiler flags")
set(CMAKE_ASM_FLAGS "${MCPU_FLAGS} -x assembler-with-cpp" CACHE INTERNAL "Assembler flags")

# Debug and release flags
set(CMAKE_C_FLAGS_DEBUG   "-Og -g3" CACHE INTERNAL "C debug compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g3" CACHE INTERNAL "C++ debug compiler flags")
set(CMAKE_ASM_FLAGS_DEBUG "-g3" CACHE INTERNAL "Assembler debug flags")

set(CMAKE_C_FLAGS_RELEASE   "-Os -DNDEBUG" CACHE INTERNAL "C release compiler flags")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG" CACHE INTERNAL "C++ release compiler flags")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "Assembler release flags")
