cmake_minimum_required(VERSION 3.12)

set(PICO_PLATFORM rp2350-arm-s)
set(PICO_BOARD pico2_w)

# Change your executable name to something creative!
set(NAME i75-boilerplate) # <-- Name your project/executable here!

include(pimoroni_pico_import.cmake)
include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add your source files
add_executable(${NAME}
    src/main.cpp # <-- Add source files here!
)

# Include required libraries
# This assumes `pimoroni-pico` is stored alongside your project
include(common/pimoroni_i2c)
include(common/pimoroni_bus)
include(libraries/pico_graphics/pico_graphics)
include(libraries/pico_vector/pico_vector)
include(libraries/interstate75/interstate75)

# Don't forget to link the libraries you need!
target_link_libraries(${NAME}
    hub75
    pico_stdlib
    pico_multicore
    pimoroni_i2c
    pico_graphics
    pico_vector
    hardware_pwm
    pico_rand
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# Enable USB UART output only
pico_enable_stdio_uart(${NAME} 0)
pico_enable_stdio_usb(${NAME} 1)

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2
    ${CMAKE_CURRENT_LIST_DIR}/README.md
    DESTINATION .
)

set(CPACK_PACKAGE_FILE_NAME ${NAME})
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)