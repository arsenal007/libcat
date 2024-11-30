# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m7)

# Compiler flags for Cortex-M7
set(MCPU_FLAGS "-mthumb -mcpu=cortex-m7")
set(VFP_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")

# Toolchain settings
set(CMAKE_C_COMPILER    arm-none-eabi-gcc.exe)
set(CMAKE_CXX_COMPILER  arm-none-eabi-g++.exe)
set(AS                  arm-none-eabi-as.exe)
set(AR                  arm-none-eabi-ar.exe)

# Locate GCC tools (optional if paths are correctly set in the environment)
find_program(GCC_PATH_ABSOLUTE ${CMAKE_C_COMPILER} ${GCC_PATH_LIST} NO_DEFAULT_PATH)
get_filename_component(ADD_TO_PATH_GCC ${GCC_PATH_ABSOLUTE} PATH) 
list(APPEND CMAKE_PROGRAM_PATH ${ADD_TO_PATH_GCC})

# Compiler and assembler flags
set(CMAKE_C_FLAGS   "${MCPU_FLAGS} ${VFP_FLAGS} -Wall -fno-builtin -std=gnu11 -fdata-sections -ffunction-sections" CACHE INTERNAL "C compiler flags")
set(CMAKE_CXX_FLAGS "${MCPU_FLAGS} ${VFP_FLAGS} -Wall -fno-exceptions -Wno-psabi -fno-builtin -std=gnu++17 -ffunction-sections -fdata-sections -fstack-usage -fno-rtti" CACHE INTERNAL "C++ compiler flags")
set(CMAKE_ASM_FLAGS "${MCPU_FLAGS} -x assembler-with-cpp" CACHE INTERNAL "Assembler flags")

# Debug and release flags
SET(CMAKE_C_FLAGS_DEBUG "-O3 -g3" CACHE INTERNAL "C debug compiler flags")
SET(CMAKE_CXX_FLAGS_DEBUG "-O3 -g3" CACHE INTERNAL "C++ debug compiler flags")
SET(CMAKE_ASM_FLAGS_DEBUG "-g3" CACHE INTERNAL "Assembler debug flags")

SET(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE INTERNAL "C release compiler flags")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE INTERNAL "C++ release compiler flags")
SET(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "Assembler release flags")
