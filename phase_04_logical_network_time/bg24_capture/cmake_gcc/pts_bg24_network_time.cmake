####################################################################
# Automatically-generated file. Do not edit!                       #
####################################################################

set(SDK_PATH "C:/Users/tasrivas/.silabs/slt/installs/conan/p/simpleca33d691c539/p")
set(COPIED_SDK_PATH "simplicity_sdk_2026.6.1")
set(PKG_PATH "C:/Users/tasrivas/.silabs/slt/installs")

add_library(slc OBJECT
    "${SDK_PATH}/boards/hardware/board/src/sl_board_control_gpio.c"
    "${SDK_PATH}/boards/hardware/board/src/sl_board_init.c"
    "${SDK_PATH}/boards/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_eusart/sl_mx25_flash_shutdown.c"
    "${SDK_PATH}/platform_core/hardware/driver/configuration_over_swo/src/sl_cos.c"
    "${SDK_PATH}/platform_core/platform/common/src/sl_assert.c"
    "${SDK_PATH}/platform_core/platform/common/src/sl_core_cortexm.c"
    "${SDK_PATH}/platform_core/platform/common/src/sl_slist.c"
    "${SDK_PATH}/platform_core/platform/common/src/sl_string.c"
    "${SDK_PATH}/platform_core/platform/common/src/sl_syscalls.c"
    "${SDK_PATH}/platform_core/platform/Device/SiliconLabs/EFR32MG24/Source/startup_efr32mg24.c"
    "${SDK_PATH}/platform_core/platform/Device/SiliconLabs/EFR32MG24/Source/system_efr32mg24.c"
    "${SDK_PATH}/platform_core/platform/driver/debug/src/sl_debug_swo.c"
    "${SDK_PATH}/platform_core/platform/driver/dma_channel/src/sl_dma_channel.c"
    "${SDK_PATH}/platform_core/platform/driver/dma_channel/src/sl_dma_descriptor_allocator.c"
    "${SDK_PATH}/platform_core/platform/driver/gpio/src/sl_gpio.c"
    "${SDK_PATH}/platform_core/platform/emlib/src/em_burtc.c"
    "${SDK_PATH}/platform_core/platform/emlib/src/em_cmu.c"
    "${SDK_PATH}/platform_core/platform/emlib/src/em_emu.c"
    "${SDK_PATH}/platform_core/platform/emlib/src/em_eusart.c"
    "${SDK_PATH}/platform_core/platform/emlib/src/em_gpio.c"
    "${SDK_PATH}/platform_core/platform/emlib/src/em_msc.c"
    "${SDK_PATH}/platform_core/platform/emlib/src/em_prs.c"
    "${SDK_PATH}/platform_core/platform/emlib/src/em_system.c"
    "${SDK_PATH}/platform_core/platform/emlib/src/em_timer.c"
    "${SDK_PATH}/platform_core/platform/emlib/src/em_usart.c"
    "${SDK_PATH}/platform_core/platform/peripheral/src/sl_hal_eusart.c"
    "${SDK_PATH}/platform_core/platform/peripheral/src/sl_hal_gpio.c"
    "${SDK_PATH}/platform_core/platform/peripheral/src/sl_hal_ldma.c"
    "${SDK_PATH}/platform_core/platform/peripheral/src/sl_hal_prs.c"
    "${SDK_PATH}/platform_core/platform/peripheral/src/sl_hal_syscfg.c"
    "${SDK_PATH}/platform_core/platform/peripheral/src/sl_hal_sysrtc.c"
    "${SDK_PATH}/platform_core/platform/peripheral/src/sl_hal_sysrtc_subsystem.c"
    "${SDK_PATH}/platform_core/platform/peripheral/src/sl_hal_system.c"
    "${SDK_PATH}/platform_core/platform/service/clock_manager/src/sl_clock_manager.c"
    "${SDK_PATH}/platform_core/platform/service/clock_manager/src/sl_clock_manager_hal_s2.c"
    "${SDK_PATH}/platform_core/platform/service/clock_manager/src/sl_clock_manager_init.c"
    "${SDK_PATH}/platform_core/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.c"
    "${SDK_PATH}/platform_core/platform/service/device_init/src/sl_device_init_dcdc_s2.c"
    "${SDK_PATH}/platform_core/platform/service/device_init/src/sl_device_init_emu_s2.c"
    "${SDK_PATH}/platform_core/platform/service/device_manager/clocks/sl_device_clock_efr32xg24.c"
    "${SDK_PATH}/platform_core/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg24.c"
    "${SDK_PATH}/platform_core/platform/service/device_manager/dma/sl_device_dma_s2.c"
    "${SDK_PATH}/platform_core/platform/service/device_manager/src/sl_device_clock.c"
    "${SDK_PATH}/platform_core/platform/service/device_manager/src/sl_device_dma.c"
    "${SDK_PATH}/platform_core/platform/service/device_manager/src/sl_device_gpio.c"
    "${SDK_PATH}/platform_core/platform/service/device_manager/src/sl_device_peripheral.c"
    "${SDK_PATH}/platform_core/platform/service/dma_manager/src/sl_dma_manager.c"
    "${SDK_PATH}/platform_core/platform/service/dma_manager/src/sl_dma_manager_hal_ldma.c"
    "${SDK_PATH}/platform_core/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.c"
    "${SDK_PATH}/platform_core/platform/service/iostream/src/sl_iostream.c"
    "${SDK_PATH}/platform_core/platform/service/iostream/src/sl_iostream_dmadrv.c"
    "${SDK_PATH}/platform_core/platform/service/iostream/src/sl_iostream_eusart.c"
    "${SDK_PATH}/platform_core/platform/service/iostream/src/sl_iostream_uart.c"
    "${SDK_PATH}/platform_core/platform/service/memory_manager/src/sl_memory_manager.c"
    "${SDK_PATH}/platform_core/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.c"
    "${SDK_PATH}/platform_core/platform/service/memory_manager/src/sl_memory_manager_pool.c"
    "${SDK_PATH}/platform_core/platform/service/memory_manager/src/sl_memory_manager_pool_common.c"
    "${SDK_PATH}/platform_core/platform/service/memory_manager/src/sl_memory_manager_region.c"
    "${SDK_PATH}/platform_core/platform/service/memory_manager/src/sl_memory_manager_retarget.c"
    "${SDK_PATH}/platform_core/platform/service/memory_manager/src/sli_memory_manager_common.c"
    "${SDK_PATH}/platform_core/platform/service/sl_main/src/sl_main_init.c"
    "${SDK_PATH}/platform_core/platform/service/sl_main/src/sl_main_init_memory.c"
    "${SDK_PATH}/platform_core/platform/service/sl_main/src/sl_main_process_action.c"
    "${SDK_PATH}/platform_core/platform/service/sleeptimer/src/sl_sleeptimer.c"
    "${SDK_PATH}/platform_core/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.c"
    "${SDK_PATH}/platform_core/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.c"
    "${SDK_PATH}/platform_core/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.c"
    "${SDK_PATH}/platform_core/platform/service/udelay/src/sl_udelay.c"
    "${SDK_PATH}/platform_core/platform/service/udelay/src/sl_udelay_armv6m_gcc.S"
    "../app.c"
    "../autogen/sl_board_default_init.c"
    "../autogen/sl_dma_manager_instances.c"
    "../autogen/sl_event_handler.c"
    "../autogen/sl_iostream_handles.c"
    "../autogen/sl_iostream_init_eusart_instances.c"
    "../main.c"
    "../pts_host_rx.c"
    "../pts_network_time.c"
    "../pts_prs_timer_capture.c"
)

target_include_directories(slc PUBLIC
   "../config"
   "../autogen"
   "../."
   "../../common"
    "${SDK_PATH}/platform_core/platform/Device/SiliconLabs/EFR32MG24/Include"
    "${SDK_PATH}/platform_core/platform/common/inc"
    "${SDK_PATH}/boards/hardware/board/inc"
    "${SDK_PATH}/platform_core/platform/service/clock_manager/inc"
    "${SDK_PATH}/platform_core/platform/service/clock_manager/src"
    "${SDK_PATH}/cmsis/Core/Include"
    "${SDK_PATH}/cmsis/Core/Include/m-profile"
    "${SDK_PATH}/cmsis/Core/Include/a-profile"
    "${SDK_PATH}/platform_core/hardware/driver/configuration_over_swo/inc"
    "${SDK_PATH}/platform_core/platform/driver/debug/inc"
    "${SDK_PATH}/platform_core/platform/service/device_manager/inc"
    "${SDK_PATH}/platform_core/platform/service/device_init/inc"
    "${SDK_PATH}/platform_core/platform/driver/dma_channel/inc"
    "${SDK_PATH}/platform_core/platform/service/dma_manager/inc"
    "${SDK_PATH}/platform_core/platform/service/dma_manager/src"
    "${SDK_PATH}/platform_core/platform/emlib/inc"
    "${SDK_PATH}/platform_core/platform/common/errno_error_codes/inc"
    "${SDK_PATH}/platform_core/platform/driver/gpio/inc"
    "${SDK_PATH}/platform_core/platform/peripheral/inc"
    "${SDK_PATH}/platform_core/platform/service/interrupt_manager/inc"
    "${SDK_PATH}/platform_core/platform/service/interrupt_manager/src"
    "${SDK_PATH}/platform_core/platform/service/interrupt_manager/inc/arm"
    "${SDK_PATH}/platform_core/platform/service/iostream/inc"
    "${SDK_PATH}/platform_core/platform/service/memory_manager/inc"
    "${SDK_PATH}/platform_core/platform/service/memory_manager/src"
    "${SDK_PATH}/boards/hardware/driver/mx25_flash_shutdown/inc/sl_mx25_flash_shutdown_eusart"
    "${SDK_PATH}/platform_core/platform/service/sl_main/inc"
    "${SDK_PATH}/platform_core/platform/service/sl_main/src"
    "${SDK_PATH}/platform_core/platform/service/sleeptimer/inc"
    "${SDK_PATH}/platform_core/platform/service/sleeptimer/src"
    "${SDK_PATH}/platform_core/platform/service/udelay/inc"
)

target_compile_definitions(slc PUBLIC
    "EFR32MG24B220F1536IM48=1"
    "SL_CODE_COMPONENT_SYSTEM=system"
    "SL_BOARD_NAME=\"BRD4187C\""
    "SL_BOARD_REV=\"A01\""
    "HARDWARE_BOARD_DEFAULT_RF_BAND_2400=1"
    "HARDWARE_BOARD_SUPPORTS_1_RF_BAND=1"
    "HARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1"
    "HFXO_FREQ=39000000"
    "SL_CODE_COMPONENT_CLOCK_MANAGER=clock_manager"
    "SL_COMPONENT_CATALOG_PRESENT=1"
    "SL_CODE_COMPONENT_DEVICE_PERIPHERAL=device_peripheral"
    "SL_CODE_COMPONENT_DMA_CHANNEL=dma_channel"
    "SL_CODE_COMPONENT_DMA_MANAGER=dma_manager"
    "SL_CODE_COMPONENT_GPIO=gpio"
    "SL_CODE_COMPONENT_HAL_COMMON=hal_common"
    "SL_CODE_COMPONENT_HAL_GPIO=hal_gpio"
    "SL_CODE_COMPONENT_HAL_LDMA=hal_ldma"
    "SL_CODE_COMPONENT_HAL_SYSRTC=hal_sysrtc"
    "SL_CODE_COMPONENT_INTERRUPT_MANAGER=interrupt_manager"
    "CMSIS_NVIC_VIRTUAL=1"
    "CMSIS_NVIC_VIRTUAL_HEADER_FILE=\"cmsis_nvic_virtual.h\""
    "SL_CODE_COMPONENT_MEMORY_MANAGER=memory_manager"
    "SL_CODE_COMPONENT_CORE=core"
    "SL_CODE_COMPONENT_SLEEPTIMER=sleeptimer"
)

target_link_libraries(slc PUBLIC
    "-Wl,--start-group"
    "gcc"
    "c"
    "m"
    "nosys"
    "-Wl,--end-group"
)
target_compile_options(slc PUBLIC
    $<$<COMPILE_LANGUAGE:C>:-mcpu=cortex-m33>
    $<$<COMPILE_LANGUAGE:C>:-mthumb>
    $<$<COMPILE_LANGUAGE:C>:-mfpu=fpv5-sp-d16>
    $<$<COMPILE_LANGUAGE:C>:-mfloat-abi=hard>
    $<$<COMPILE_LANGUAGE:C>:-mcmse>
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
    $<$<COMPILE_LANGUAGE:C>:-Os>
    $<$<COMPILE_LANGUAGE:C>:-fdata-sections>
    $<$<COMPILE_LANGUAGE:C>:-ffunction-sections>
    $<$<COMPILE_LANGUAGE:C>:-fomit-frame-pointer>
    $<$<COMPILE_LANGUAGE:C>:-g>
    $<$<COMPILE_LANGUAGE:C>:--specs=nano.specs>
    $<$<COMPILE_LANGUAGE:C>:-fno-lto>
    $<$<COMPILE_LANGUAGE:CXX>:-mcpu=cortex-m33>
    $<$<COMPILE_LANGUAGE:CXX>:-mthumb>
    $<$<COMPILE_LANGUAGE:CXX>:-mfpu=fpv5-sp-d16>
    $<$<COMPILE_LANGUAGE:CXX>:-mfloat-abi=hard>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-mcmse>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
    $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
    $<$<COMPILE_LANGUAGE:CXX>:-Os>
    $<$<COMPILE_LANGUAGE:CXX>:-fdata-sections>
    $<$<COMPILE_LANGUAGE:CXX>:-ffunction-sections>
    $<$<COMPILE_LANGUAGE:CXX>:-fomit-frame-pointer>
    $<$<COMPILE_LANGUAGE:CXX>:-g>
    $<$<COMPILE_LANGUAGE:CXX>:--specs=nano.specs>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-lto>
    $<$<COMPILE_LANGUAGE:ASM>:-mcpu=cortex-m33>
    $<$<COMPILE_LANGUAGE:ASM>:-mthumb>
    $<$<COMPILE_LANGUAGE:ASM>:-mfpu=fpv5-sp-d16>
    $<$<COMPILE_LANGUAGE:ASM>:-mfloat-abi=hard>
    "$<$<COMPILE_LANGUAGE:ASM>:SHELL:-x assembler-with-cpp>"
)

set(post_build_command )
set_property(TARGET slc PROPERTY C_STANDARD 17)
set_property(TARGET slc PROPERTY CXX_STANDARD 17)
set_property(TARGET slc PROPERTY CXX_EXTENSIONS OFF)

target_link_options(slc INTERFACE
    -mcpu=cortex-m33
    -mthumb
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    "-T${CMAKE_CURRENT_LIST_DIR}/../autogen/linkerfile.ld"
    --specs=nano.specs
    -Wl,-Map=$<TARGET_FILE_DIR:pts_bg24_network_time>/pts_bg24_network_time.map
    "SHELL:-Wl,--wrap=_free_r -Wl,--wrap=_malloc_r -Wl,--wrap=_calloc_r -Wl,--wrap=_realloc_r"
    -fno-lto
    -Wl,--gc-sections
)

# BEGIN_SIMPLICITY_STUDIO_METADATA=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END_SIMPLICITY_STUDIO_METADATA