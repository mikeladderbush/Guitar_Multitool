cmake_minimum_required(VERSION 3.15)

project(Guitar_Multitool VERSION 0.1 LANGUAGE C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_compile_options(GtrMt PRIVATE -Wall -Wextra -Wconversion -Werror -g)

option(SANITIZE "Build with AddressSanitizer and UBSan" OFF)
if(SANITIZE)
    add_compile_options(-fsanitize=address,undefined
    -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

include(CTest)

add_subdirectory(src)

add_executable(GtrMt main.c)
target_link_libraries(GtrMt PRIVATE core)

add_subdirectory(test)