list_current_sources_and_subdirs(SRCS DIRS)

add_library(System.Machine.X86 OBJECT ${SRCS} EntryPoint.S)
target_include_directories(System.Machine.X86
        PUBLIC $<TARGET_PROPERTY:Kernel,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(System.Machine.X86
        PUBLIC $<TARGET_PROPERTY:Kernel,INTERFACE_COMPILE_DEFINITIONS>
)

foreach (subdir ${DIRS})
    add_subdirectory(${subdir})
endforeach ()

set(SYSTEM_MACHINE_X86_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Linker.ld PARENT_SCOPE)