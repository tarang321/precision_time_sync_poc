####################################################################
# Automatically-generated file. Do not edit!                       #
####################################################################

set(SDK_PATH "C:/Users/tasrivas/.silabs/slt/installs/conan/p/simpl508ee6c1a6569/p")
set(COPIED_SDK_PATH "simplicity_sdk_2026.6.0")
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

# BEGIN_SIMPLICITY_STUDIO_METADATA=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END_SIMPLICITY_STUDIO_METADATA