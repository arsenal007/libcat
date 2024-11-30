
# Set the system name and processor type
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR xtensa-lx6)

# Define Xtensa-specific compiler flags
set(MCPU_FLAGS "-mlongcalls -mtext-section-literals")
set(EXTRA_FLAGS "-ffunction-sections -fdata-sections -Wall -fstrict-volatile-bitfields")

# Toolchain settings
set(CMAKE_C_COMPILER    xtensa-esp32-elf-gcc)
set(CMAKE_CXX_COMPILER  xtensa-esp32-elf-g++)
set(AS                  xtensa-esp32-elf-as)
set(AR                  xtensa-esp32-elf-ar)

# Locate GCC tools (optional if paths are correctly set in the environment)
find_program(GCC_PATH_ABSOLUTE ${CMAKE_C_COMPILER} ${GCC_PATH_LIST} NO_DEFAULT_PATH)
get_filename_component(ADD_TO_PATH_GCC ${GCC_PATH_ABSOLUTE} PATH) 
list(APPEND CMAKE_PROGRAM_PATH ${ADD_TO_PATH_GCC})

# Compiler and assembler flags
set(CMAKE_C_FLAGS   "${MCPU_FLAGS} ${EXTRA_FLAGS} -std=gnu11" CACHE INTERNAL "C compiler flags")
set(CMAKE_CXX_FLAGS "${MCPU_FLAGS} ${EXTRA_FLAGS} -std=gnu++17 -fno-exceptions -fno-rtti" CACHE INTERNAL "C++ compiler flags")
set(CMAKE_ASM_FLAGS "${MCPU_FLAGS}" CACHE INTERNAL "Assembler flags")

# Debug and release flags
set(CMAKE_C_FLAGS_DEBUG   "-Og -g3" CACHE INTERNAL "C debug compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g3" CACHE INTERNAL "C++ debug compiler flags")
set(CMAKE_ASM_FLAGS_DEBUG "-g3" CACHE INTERNAL "Assembler debug flags")

set(CMAKE_C_FLAGS_RELEASE   "-Os -DNDEBUG" CACHE INTERNAL "C release compiler flags")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG" CACHE INTERNAL "C++ release compiler flags")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "Assembler release flags")
