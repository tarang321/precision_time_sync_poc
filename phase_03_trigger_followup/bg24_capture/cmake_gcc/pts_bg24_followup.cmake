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
    -Wl,-Map=$<TARGET_FILE_DIR:pts_bg24_followup>/pts_bg24_followup.map
    "SHELL:-Wl,--wrap=_free_r -Wl,--wrap=_malloc_r -Wl,--wrap=_calloc_r -Wl,--wrap=_realloc_r"
    -fno-lto
    -Wl,--gc-sections
)

# BEGIN_SIMPLICITY_STUDIO_METADATA=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END_SIMPLICITY_STUDIO_METADATA