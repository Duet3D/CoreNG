list(APPEND SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/adc/adc.c"
    "${CMAKE_CURRENT_LIST_DIR}/variant.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/system_sam4s.c"
    "${CMAKE_CURRENT_LIST_DIR}/startup_sam4s.c"
    "${CMAKE_CURRENT_LIST_DIR}/exceptions.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/common/services/clock/sam4s/sysclk.c")

list(REMOVE_ITEM SRCS
    "asf/sam/drivers/rswdt/rswdt.c"
    "asf/sam/drivers/afec/afec.c"
    "asf/sam/drivers/dmac/dmac.c"
    "asf/sam/drivers/cmcc/cmcc.c"
)

list(APPEND INCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/utils/cmsis/sam4s/include")

set(LINKER_SCRIPTS ${CMAKE_CURRENT_LIST_DIR}/linker_scripts/gcc/flash.ld)
