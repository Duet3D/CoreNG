list(APPEND SRCS
    "${CMAKE_CURRENT_LIST_DIR}/variant.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/system_sam4e.c"
    "${CMAKE_CURRENT_LIST_DIR}/startup_sam4e.c"
    "${CMAKE_CURRENT_LIST_DIR}/exceptions.c"

    "${CMAKE_CURRENT_SOURCE_DIR}/asf/common/services/clock/sam4e/sysclk.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/rswdt/rswdt.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/afec/afec.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/dmac/dmac.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/cmcc/cmcc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/twi/twi.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/pdc/pdc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/udp/udp_device.c"
)

list(APPEND INCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/utils/cmsis/sam4e/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/dmac")

set(LINKER_SCRIPTS ${CMAKE_CURRENT_LIST_DIR}/linker_scripts/gcc/flash.ld)