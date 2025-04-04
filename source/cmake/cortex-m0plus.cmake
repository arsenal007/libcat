# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m0plus)

# Flags specific to Cortex-M0+ (no FPU!)
set(MCPU_FLAGS "-mthumb -mcpu=cortex-m0plus")

# No VFP flags for Cortex-M0+
set(ARCH_FLAGS "${MCPU_FLAGS}")

# Include the generic GCC toolchain logic
include("${CMAKE_CURRENT_LIST_DIR}/gcc.cmake")
