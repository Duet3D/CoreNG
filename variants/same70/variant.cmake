list(APPEND SRCS
    "${CMAKE_CURRENT_LIST_DIR}/variant.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/system_same70.c"
    "${CMAKE_CURRENT_LIST_DIR}/startup_same70.c"

    "${CMAKE_CURRENT_SOURCE_DIR}/asf/common/services/clock/same70/sysclk.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/common/services/usb/udc/udc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/components/ethernet_phy/ksz8081rna/ethernet_phy.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/xdmac/xdmac.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/gmac/gmac_phy.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/rswdt/rswdt.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/afec/afec.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/usbhs/usbhs_device.c"
)

list(APPEND INCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/utils/cmsis/same70/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/xdmac"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/gmac"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/drivers/emac"
    "${CMAKE_CURRENT_SOURCE_DIR}/asf/sam/components/ethernet_phy/ksz8081rna"
    )

set(LINKER_SCRIPTS ${CMAKE_CURRENT_LIST_DIR}/linker_scripts/gcc/flash.ld)