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
    -Wl,-Map=$<TARGET_FILE_DIR:pts_bg24_wifi_cmd>/pts_bg24_wifi_cmd.map
    "SHELL:-Wl,--wrap=_free_r -Wl,--wrap=_malloc_r -Wl,--wrap=_calloc_r -Wl,--wrap=_realloc_r"
    -fno-lto
    -Wl,--gc-sections
)

# BEGIN_SIMPLICITY_STUDIO_METADATA=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=END_SIMPLICITY_STUDIO_METADATA