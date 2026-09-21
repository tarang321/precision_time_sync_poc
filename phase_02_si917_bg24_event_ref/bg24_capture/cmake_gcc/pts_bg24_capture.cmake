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

# BEGIN_SIMPLICITY_STUDIO_METADATA=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=END_SIMPLICITY_STUDIO_METADATA