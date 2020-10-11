list_current_sources_and_subdirs(SRCS DIRS)

add_library(System.FileSystem.FAT32 STATIC ${SRCS})
target_include_directories(System.FileSystem.FAT32
        PRIVATE $<TARGET_PROPERTY:Kernel,INTERFACE_INCLUDE_DIRECTORIES>
)

foreach (subdir ${DIRS})
    add_subdirectory(${subdir})
endforeach ()
