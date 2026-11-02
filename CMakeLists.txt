cmake_minimum_required(VERSION 3.21)

project(Guitar_Multitool VERSION 0.1 LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_compile_options(GtrMt PRIVATE -Wall -Wextra -Wconversion -Werror -O0 -g3)

add_subdirectory(src)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32G031GBUX_FLASH.ld)

add_executable(firmware
    main.c
    startup_stm32g031xx.s
)

set_target_properties(firmware PROPERTIES
    SUFFIX ".elf"
    LINK_DEPENDS ${LINKER_SCRIPT}
)

target_link_libraries(firmware PRIVATE core hal)

target_link_options(firmware PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=$<TARGET_FILE_DIR:firmware>/firmware.map
    -Wl,--cref
    -Wl,--print-memory-usage
)

add_custom_command(TARGET firmware POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:firmware> $<TARGET_FILE_DIR:firmware>/firmware.bin
     COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:firmware> $<TARGET_FILE_DIR:firmware>/firmware.hex
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:firmware>
    VERBATIM
)