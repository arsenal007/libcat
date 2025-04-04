# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m33)

# Base CPU flags
set(MCPU_FLAGS "-mthumb -mcpu=cortex-m33")

# TrustZone support (optional)
set(TRUSTZONE_FLAGS "-mcmse")

# Floating point support (optional)
set(VFP_FLAGS "-mfloat-abi=hard -mfpu=fpv5-sp-d16")

# Combine all flags into one
set(ARCH_FLAGS "${MCPU_FLAGS} ${TRUSTZONE_FLAGS} ${VFP_FLAGS}")

include("${CMAKE_CURRENT_LIST_DIR}/gcc.cmake")
