cmake_minimum_required(VERSION 3.10)
project(CoreNG)

add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>")

if(BOARD STREQUAL "DuetNG")
    set(variant "duetNG")
endif()

set(SRCS
    "libraries/Wire/Wire.cpp"
    "libraries/Storage/ctrl_access.c"
    "libraries/Storage/sd_mmc.c"
    "libraries/Storage/sd_mmc_mem.c"
    "libraries/Storage/sd_mmc_spi.c"
    "libraries/SharedSpi/SharedSpi.cpp"
    "libraries/Flash/DueFlashStorage.cpp"
    "cores/arduino/USB/USBSerial.cpp"
    "cores/arduino/AnalogIn.cpp"
    "cores/arduino/AnalogOut.cpp"
    "cores/arduino/Print.cpp"
    "cores/arduino/Reset.cpp"
    "cores/arduino/RingBuffer.cpp"
    "cores/arduino/Stream.cpp"
    "cores/arduino/UARTClass.cpp"
    "cores/arduino/USARTClass.cpp"
    "cores/arduino/WMath.cpp"
    "cores/arduino/WInterrupts.cpp"
    "cores/arduino/abi.cpp"
    "cores/arduino/hooks.c"
    "cores/arduino/itoa.c"
    "cores/arduino/new.cpp"
    "cores/arduino/watchdog.cpp"
    "cores/arduino/wiring.c"
    "cores/arduino/wiring_digital.cpp"
    "asf/sam/services/flash_efc/flash_efc.c"
    "asf/sam/drivers/wdt/wdt.c"
    "asf/sam/drivers/usart/usart.c"
    "asf/sam/drivers/udp/udp_device.c"
    "asf/sam/drivers/uart/uart.c"
    "asf/sam/drivers/twi/twi.c"
    "asf/sam/drivers/tc/tc.c"
    "asf/sam/drivers/supc/supc.c"
    "asf/sam/drivers/spi/spi.c"
    "asf/sam/drivers/rswdt/rswdt.c"
    "asf/sam/drivers/rstc/rstc.c"
    "asf/sam/drivers/pwm/pwm.c"
    "asf/sam/drivers/pmc/pmc.c"
    "asf/sam/drivers/pmc/sleep.c"
    "asf/sam/drivers/pio/pio.c"
    "asf/sam/drivers/pdc/pdc.c"
    "asf/sam/drivers/matrix/matrix.c"
    "asf/sam/drivers/hsmci/hsmci.c"
    "asf/sam/drivers/gpbr/gpbr.c"
    "asf/sam/drivers/efc/efc.c"
    "asf/sam/drivers/dmac/dmac.c"
    "asf/sam/drivers/dacc/dacc.c"
    "asf/sam/drivers/cmcc/cmcc.c"
    "asf/sam/drivers/chipid/chipid.c"
    "asf/sam/drivers/afec/afec.c"
    "asf/common/utils/interrupt/interrupt_sam_nvic.c"
    "asf/common/services/usb/udc/udc.c"
    "asf/common/services/usb/class/cdc/device/udi_cdc.c"
    "asf/common/services/usb/class/cdc/device/udi_cdc_desc.c"
    "asf/common/services/sleepmgr/sam/sleepmgr.c"
)

set(INCLUDE_DIRS
    "cores/arduino"
    "libraries/Wire"
    "libraries/Storage"
    "libraries/SharedSpi"
    "asf"
    "asf/sam/utils"
    "asf/sam/utils/header_files"
    "asf/sam/utils/preprocessor"
    "asf/sam/drivers"
    "asf/sam/drivers/dmac"
    "asf/sam/drivers/efc"
    "asf/sam/drivers/hsmci"
    "asf/sam/drivers/pio"
    "asf/sam/drivers/pmc"
    "asf/sam/drivers/spi"
    "asf/sam/drivers/usart"
    "asf/common/utils"
    "asf/common/services/clock"
    "asf/common/services/ioport"
    "asf/common/services/sleepmgr"
    "asf/common/services/usb"
    "asf/common/services/usb/udc"
    "asf/common/services/usb/class/cdc"
    "asf/common/services/usb/class/cdc/device"
    "asf/thirdparty/CMSIS/Include"
)

# Pull in variant-specific build specifications. Appends source
# files and include dirs to SRCS and INCLUDE_DIRS, respectively.
include ("variants/${variant}/variant.cmake")

add_library(CoreNG ${SRCS})
target_include_directories(CoreNG PUBLIC ${INCLUDE_DIRS})

# LINKER_SCRIPTS should be set by variant.cmake.
foreach(linker_script ${LINKER_SCRIPTS})
    target_link_libraries(CoreNG INTERFACE "-T ${linker_script}")
endforeach()

target_compile_definitions(CoreNG PRIVATE "-DUDD_ENABLE" "-DUDD_NO_SLEEP_MGR")

# Link flags to propagate to RepRapFirmware
target_link_libraries(CoreNG INTERFACE "-Wl,--entry=Reset_Handler")
add_library(syscalls_obj OBJECT "cores/arduino/syscalls.c")
target_include_directories(syscalls_obj PRIVATE ${INCLUDE_DIRS})
target_link_libraries(CoreNG INTERFACE "$<TARGET_OBJECTS:syscalls_obj>")
add_dependencies(CoreNG syscalls_obj)