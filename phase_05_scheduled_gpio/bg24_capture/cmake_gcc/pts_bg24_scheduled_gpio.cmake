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
    "../pts_schedule_gpio.c"
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
    -Wl,-Map=$<TARGET_FILE_DIR:pts_bg24_scheduled_gpio>/pts_bg24_scheduled_gpio.map
    "SHELL:-Wl,--wrap=_free_r -Wl,--wrap=_malloc_r -Wl,--wrap=_calloc_r -Wl,--wrap=_realloc_r"
    -fno-lto
    -Wl,--gc-sections
)

# BEGIN_SIMPLICITY_STUDIO_METADATA=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END_SIMPLICITY_STUDIO_METADATA