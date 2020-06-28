list(APPEND SRCS
    "${CMAKE_CURRENT_LIST_DIR}/variant.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/system_sam4e.c"
    "${CMAKE_CURRENT_LIST_DIR}/startup_sam4e.c"
    "${CMAKE_CURRENT_LIST_DIR}/exceptions.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/common/services/clock/sam4e/sysclk.c")

list(APPEND INCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/utils/cmsis/sam4e/include")

set(LINKER_SCRIPTS ${CMAKE_CURRENT_LIST_DIR}/linker_scripts/gcc/flash.ld)