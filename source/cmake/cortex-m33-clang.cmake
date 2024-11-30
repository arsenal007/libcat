# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m33)

# Compiler flags for Cortex-M33
set(MCPU_FLAGS "-mthumb -mcpu=cortex-m33")
set(TRUSTZONE_FLAGS "-mcmse")  # Enable TrustZone if required
set(VFP_FLAGS "-mfloat-abi=hard -mfpu=fpv5-sp-d16")  # Optional: Enable floating-point unit

# Toolchain settings for Clang
set(CMAKE_C_COMPILER    clang)
set(CMAKE_CXX_COMPILER  clang++)
set(AS                  clang)
set(AR                  llvm-ar)

# Set the target for cross-compilation
set(CLANG_TARGET "--target=arm-none-eabi")

# Compiler and assembler flags
set(CMAKE_C_FLAGS   "${CLANG_TARGET} ${MCPU_FLAGS} ${VFP_FLAGS} ${TRUSTZONE_FLAGS} -Wall -fno-builtin -std=gnu11 -fdata-sections -ffunction-sections" CACHE INTERNAL "C compiler flags")
set(CMAKE_CXX_FLAGS "${CLANG_TARGET} ${MCPU_FLAGS} ${VFP_FLAGS} ${TRUSTZONE_FLAGS} -Wall -fno-exceptions -fno-rtti -std=gnu++17 -fdata-sections -ffunction-sections" CACHE INTERNAL "C++ compiler flags")
set(CMAKE_ASM_FLAGS "${CLANG_TARGET} ${MCPU_FLAGS} ${TRUSTZONE_FLAGS} -x assembler-with-cpp" CACHE INTERNAL "Assembler flags")

# Debug and release flags
set(CMAKE_C_FLAGS_DEBUG   "-Og -g3" CACHE INTERNAL "C debug compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g3" CACHE INTERNAL "C++ debug compiler flags")
set(CMAKE_ASM_FLAGS_DEBUG "-g3" CACHE INTERNAL "Assembler debug flags")

set(CMAKE_C_FLAGS_RELEASE   "-Oz -DNDEBUG" CACHE INTERNAL "C release compiler flags")
set(CMAKE_CXX_FLAGS_RELEASE "-Oz -DNDEBUG" CACHE INTERNAL "C++ release compiler flags")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "Assembler release flags")

# Linker settings
set(CMAKE_EXE_LINKER_FLAGS "${CLANG_TARGET} ${MCPU_FLAGS} ${VFP_FLAGS} -Wl,--gc-sections -Wl,--start-group -lc -lm -Wl,--end-group" CACHE INTERNAL "Linker flags")
