# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv32)

# Compiler flags for RISC-V
set(MCPU_FLAGS "-march=rv32imac -mabi=ilp32")
set(EXTRA_FLAGS "-ffunction-sections -fdata-sections -Wall -Wextra -fno-builtin")

# Toolchain settings for Clang
set(CMAKE_C_COMPILER    clang)
set(CMAKE_CXX_COMPILER  clang++)
set(AS                  clang)
set(AR                  llvm-ar)

# Locate Clang tools (optional if paths are correctly set in the environment)
find_program(CLANG_PATH_ABSOLUTE ${CMAKE_C_COMPILER} ${GCC_PATH_LIST} NO_DEFAULT_PATH)
get_filename_component(ADD_TO_PATH_CLANG ${CLANG_PATH_ABSOLUTE} PATH) 
list(APPEND CMAKE_PROGRAM_PATH ${ADD_TO_PATH_CLANG})

# Compiler and assembler flags
set(CMAKE_C_FLAGS   "${MCPU_FLAGS} ${EXTRA_FLAGS} -std=gnu11" CACHE INTERNAL "C compiler flags")
set(CMAKE_CXX_FLAGS "${MCPU_FLAGS} ${EXTRA_FLAGS} -std=gnu++17 -fno-exceptions -fno-rtti" CACHE INTERNAL "C++ compiler flags")
set(CMAKE_ASM_FLAGS "${MCPU_FLAGS}" CACHE INTERNAL "Assembler flags")

# Debug and release flags
set(CMAKE_C_FLAGS_DEBUG   "-Og -g3" CACHE INTERNAL "C debug compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g3" CACHE INTERNAL "C++ debug compiler flags")
set(CMAKE_ASM_FLAGS_DEBUG "-g3" CACHE INTERNAL "Assembler debug flags")

set(CMAKE_C_FLAGS_RELEASE   "-Oz -DNDEBUG" CACHE INTERNAL "C release compiler flags")
set(CMAKE_CXX_FLAGS_RELEASE "-Oz -DNDEBUG" CACHE INTERNAL "C++ release compiler flags")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "Assembler release flags")

# Linker settings
set(CMAKE_EXE_LINKER_FLAGS "${MCPU_FLAGS} -Wl,--gc-sections -Wl,--start-group -lc -lm -Wl,--end-group" CACHE INTERNAL "Linker flags")
