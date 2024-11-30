# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m7)

# Compiler flags for Cortex-M7
set(MCPU_FLAGS "-mthumb -mcpu=cortex-m7")
set(VFP_FLAGS "-mfloat-abi=hard -mfpu=fpv5-d16")  # Enable floating-point unit

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
set(CMAKE_C_FLAGS   "${MCPU_FLAGS} ${VFP_FLAGS} -Wall -fno-builtin -std=gnu11 -fdata-sections -ffunction-sections" CACHE INTERNAL "C compiler flags")
set(CMAKE_CXX_FLAGS "${MCPU_FLAGS} ${VFP_FLAGS} -Wall -fno-exceptions -fno-rtti -std=gnu++17 -fdata-sections -ffunction-sections" CACHE INTERNAL "C++ compiler flags")
set(CMAKE_ASM_FLAGS "${MCPU_FLAGS} -x assembler-with-cpp" CACHE INTERNAL "Assembler flags")

# Debug and release flags
set(CMAKE_C_FLAGS_DEBUG   "-Og -g3" CACHE INTERNAL "C debug compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g3" CACHE INTERNAL "C++ debug compiler flags")
set(CMAKE_ASM_FLAGS_DEBUG "-g3" CACHE INTERNAL "Assembler debug flags")

set(CMAKE_C_FLAGS_RELEASE   "-Oz -DNDEBUG" CACHE INTERNAL "C release compiler flags")
set(CMAKE_CXX_FLAGS_RELEASE "-Oz -DNDEBUG" CACHE INTERNAL "C++ release compiler flags")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "Assembler release flags")

# Linker settings
set(CMAKE_EXE_LINKER_FLAGS "${MCPU_FLAGS} ${VFP_FLAGS} -Wl,--gc-sections -Wl,--start-group -lc -lm -Wl,--end-group" CACHE INTERNAL "Linker flags")
