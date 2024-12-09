cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(wifi_ap C CXX ASM)

pico_sdk_init()

# 定义源文件所在目录的变量
set(SOURCE_DIR src)

add_executable(wifi_ap
        src/main.c
)

# pull in common dependencies
target_link_libraries(wifi_ap 
                pico_stdlib
                pico_binary_info
                hardware_i2c
                hardware_uart
)

target_include_directories(wifi_ap PRIVATE
                src
                src/header
                src/ssd1306
)
            
target_sources(wifi_ap PUBLIC
        src/wifi_ap.c
        src/uart.c
        src/ssd1306/i2c.c
)

# create map/bin/hex file etc.
pico_enable_stdio_uart(wifi_ap 1)

pico_add_extra_outputs(wifi_ap)