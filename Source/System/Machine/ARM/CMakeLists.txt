list_current_sources_and_subdirs(SRCS DIRS)

add_library(System.Machine.ARM OBJECT ${SRCS})
target_include_directories(System.Machine.ARM
        PUBLIC $<TARGET_PROPERTY:Kernel,INTERFACE_INCLUDE_DIRECTORIES>
        )
target_compile_definitions(System.Machine.ARM
        PUBLIC $<TARGET_PROPERTY:Kernel,INTERFACE_COMPILE_DEFINITIONS>
        )

foreach (subdir ${DIRS})
    add_subdirectory(${subdir})
endforeach ()

set(SYSTEM_MACHINE_ARM_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Linker.ld PARENT_SCOPE)