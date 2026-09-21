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
    -Wl,-Map=$<TARGET_FILE_DIR:pts_bg24_followup>/pts_bg24_followup.map
    "SHELL:-Wl,--wrap=_free_r -Wl,--wrap=_malloc_r -Wl,--wrap=_calloc_r -Wl,--wrap=_realloc_r"
    -fno-lto
    -Wl,--gc-sections
)

# BEGIN_SIMPLICITY_STUDIO_METADATA=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END_SIMPLICITY_STUDIO_METADATA