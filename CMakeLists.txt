cmake_minimum_required(VERSION 3.3)
include(CMake/Machine/X86.cmake)
project(Kernel ASM C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set(CMAKE_CXX_STANDARD 17)

set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Vendor)
set(CMAKE_C_LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/Source/System/Machine/${ARCH_TARGET}/Linker.ld -S -nostdlib ${OPT}")

add_subdirectory(Vendor/function2)
add_subdirectory(Vendor/continuable)

include(SourceAndDirectoryListing)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
list_sources_and_subdirs(SRCS DIRS ${SRC_DIR})

add_library(Kernel OBJECT ${SRCS})
target_include_directories(Kernel
        PUBLIC ${SRC_DIR}
        PUBLIC ${SRC_DIR}/System/Support/C
        PUBLIC $<TARGET_PROPERTY:System.C++,INTERFACE_INCLUDE_DIRECTORIES>
        
        PUBLIC ${TOOLCHAIN_DIR}/lib/clang/5.0.0/include
        PUBLIC ${VENDOR_DIR}/stdlib
        
        PUBLIC ${VENDOR_DIR}/function2/include
        PUBLIC ${VENDOR_DIR}/continuable/include
)
target_compile_definitions(Kernel
        PUBLIC $<TARGET_PROPERTY:System.C++,INTERFACE_COMPILE_DEFINITIONS>
)

foreach(subdir ${DIRS})
    add_subdirectory(${subdir})
endforeach()

add_executable(System
        $<TARGET_OBJECTS:System.Machine.${ARCH_TARGET}>
        $<TARGET_OBJECTS:Kernel>
        $<TARGET_OBJECTS:System.C++>
        $<TARGET_OBJECTS:System.C++ABI>
)

add_custom_command(TARGET System
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/Tools/multiboot_patch.py "$<TARGET_FILE:System>"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(run qemu-system-${QEMU_TARGET}
        -kernel ${CMAKE_BINARY_DIR}/System
        -m 512 -d in_asm
)
add_dependencies(run System)
