cmake_minimum_required(VERSION 3.3)
project(kernel)
enable_language(ASM)

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -target i386-elf -ffreestanding")
set(CMAKE_C_FLAGS "-target i386-elf -ffreestanding -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++14")

set(CMAKE_C_LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker.ld -nostdlib ")
SET(CMAKE_C_LINK_EXECUTABLE "/usr/local/i386elfgcc/bin/i386-elf-ld <CMAKE_C_LINK_FLAGS> <OBJECTS>  -o <TARGET>")
SET(CMAKE_CXX_LINK_EXECUTABLE "/usr/local/i386elfgcc/bin/i386-elf-ld <CMAKE_C_LINK_FLAGS> <OBJECTS>  -o <TARGET>")

set(SOURCE_FILES main.cpp entry_point.S Kernel/Console.cpp Kernel/Console.h)
add_executable(kernel.bin ${SOURCE_FILES})

add_custom_target(run qemu-system-i386 -kernel ${CMAKE_BINARY_DIR}/kernel.bin)