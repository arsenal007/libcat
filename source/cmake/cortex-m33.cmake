# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m33)

# Compiler flags for Cortex-M33
set(MCPU_FLAGS "-mthumb -mcpu=cortex-m33")
set(TRUSTZONE_FLAGS "-mcmse")  # Enable TrustZone if required
set(VFP_FLAGS "-mfloat-abi=hard -mfpu=fpv5-sp-d16")  # Optional: Enable floating-point unit

# Toolchain settings
set(CMAKE_C_COMPILER    arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER  arm-none-eabi-g++)
set(AS                  arm-none-eabi-as)
set(AR                  arm-none-eabi-ar)


# Compiler and assembler flags
set(CMAKE_C_FLAGS   "${MCPU_FLAGS} ${VFP_FLAGS} ${TRUSTZONE_FLAGS} -Wall -fno-builtin -std=gnu11 -fdata-sections -ffunction-sections" CACHE INTERNAL "C compiler flags")
set(CMAKE_CXX_FLAGS "${MCPU_FLAGS} ${VFP_FLAGS} ${TRUSTZONE_FLAGS} -Wall -fno-exceptions -Wno-psabi -fno-builtin -std=gnu++17 -ffunction-sections -fdata-sections -fstack-usage -fno-rtti" CACHE INTERNAL "C++ compiler flags")
set(CMAKE_ASM_FLAGS "${MCPU_FLAGS} ${TRUSTZONE_FLAGS} -x assembler-with-cpp" CACHE INTERNAL "Assembler flags")

# Debug and release flags
SET(CMAKE_C_FLAGS_DEBUG "-O3 -g3" CACHE INTERNAL "C debug compiler flags")
SET(CMAKE_CXX_FLAGS_DEBUG "-O3 -g3" CACHE INTERNAL "C++ debug compiler flags")
SET(CMAKE_ASM_FLAGS_DEBUG "-g3" CACHE INTERNAL "Assembler debug flags")

SET(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE INTERNAL "C release compiler flags")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE INTERNAL "C++ release compiler flags")
SET(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "Assembler release flags")
