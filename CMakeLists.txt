cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/freertos)
include(FreeRTOS_Kernel_import.cmake)

project(pico-logger)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(pico-logger
    main.cpp
    hw_config.c
    tasks/sdcard_task.cpp
    tasks/dut_uart_task.cpp
)

add_subdirectory(lib/FatFs_SPI)

target_include_directories(pico-logger PRIVATE
    tasks
    config
)


target_link_libraries(pico-logger 
    pico_stdlib
    FatFs_SPI
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap1
)

pico_add_extra_outputs(pico-logger)
