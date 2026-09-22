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
    -Wl,-Map=$<TARGET_FILE_DIR:pts_bg24_capture>/pts_bg24_capture.map
    "SHELL:-Wl,--wrap=_free_r -Wl,--wrap=_malloc_r -Wl,--wrap=_calloc_r -Wl,--wrap=_realloc_r"
    -fno-lto
    -Wl,--gc-sections
)

# BEGIN_SIMPLICITY_STUDIO_METADATA=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END_SIMPLICITY_STUDIO_METADATA