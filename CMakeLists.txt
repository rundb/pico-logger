cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pico-logger)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(pico-logger
    main.c
    hw_config.c
)

add_subdirectory(lib/FatFs_SPI)

target_link_libraries(pico-logger 
    pico_stdlib
    FatFs_SPI
)

pico_add_extra_outputs(pico-logger)
