# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m7)

# Compiler flags for Cortex-M7
set(MCPU_FLAGS "-mthumb -mcpu=cortex-m7")
set(VFP_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")

set(ARCH_FLAGS "${MCPU_FLAGS} ${VFP_FLAGS}")

include("${CMAKE_CURRENT_LIST_DIR}/gcc.cmake")