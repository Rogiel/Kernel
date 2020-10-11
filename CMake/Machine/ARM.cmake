set(ARCH_TARGET ARM)
set(QEMU_TARGET arm)
set(TARGET arm-elf)

set(TOOLCHAIN_DIR ${CMAKE_CURRENT_LIST_DIR}/../../Toolchain)
set(LTO -flto)
set(OPT "-O1")
set(VECTORIZE "-fno-vectorize -std=c++17 -mfloat-abi=hard")

set(FLAGS "-target ${TARGET} -fno-exceptions -ffreestanding -fcoroutines-ts -nostdlib ${OPT} ${LTO} ${VECTORIZE}")

set(CMAKE_SYSTEM_NAME Kernel)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Toolchain)
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)

set(CMAKE_SYSROOT)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_DIR}/bin/clang)
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${FLAGS}")

set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS}")

set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")

SET(CMAKE_CXX_LINK_EXECUTABLE "${TOOLCHAIN_DIR}/bin/ld.lld <CMAKE_C_LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -o <TARGET>")
set(CMAKE_CXX_LINK_EXECUTABLE "${TOOLCHAIN_DIR}/bin/ld.lld <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
