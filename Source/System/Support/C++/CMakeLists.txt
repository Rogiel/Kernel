set(LIBCXXABI_INC_DIR ${VENDOR_DIR}/libcxxabi/include)
set(LIBCXXABI_SRC_DIR ${VENDOR_DIR}/libcxxabi/src)

set(LIBCXX_INC_DIR ${VENDOR_DIR}/libcxx/include)
set(LIBCXX_SRC_DIR ${VENDOR_DIR}/libcxx/src)

set(FLAGS
        _LIBCPP_HAS_NO_THREADS
        __ELF__
        _LIBCPP_STD_VER=17
        _LIBCXX_NO_EXCEPTIONS=1
        _LIBCPP_NO_EXCEPTIONS=1
        _LIBCXXABI_NO_EXCEPTIONS=1
        _LIBCXXABI_HAS_NO_THREADS=1
#        HAVE___CXA_THREAD_ATEXIT_IMPL
        _LIBCPP_HAS_NO_STDIN
        _LIBCPP_HAS_NO_STDOUT
        _LIBCPP_HAS_NO_THREADS
        _LIBCPP_HAS_NO_MONOTONIC_CLOCK
        _LIBCPP_HAS_NO_THREAD_UNSAFE_C_FUNCTIONS
)

add_library(System.C++ OBJECT
        ${LIBCXX_SRC_DIR}/string.cpp
        ${LIBCXX_SRC_DIR}/memory.cpp
        ${LIBCXX_SRC_DIR}/vector.cpp
        ${LIBCXX_SRC_DIR}/utility.cpp
        ${LIBCXX_SRC_DIR}/variant.cpp
        ${LIBCXX_SRC_DIR}/functional.cpp
        ${LIBCXX_SRC_DIR}/any.cpp
        ${LIBCXX_SRC_DIR}/bind.cpp
        ${LIBCXX_SRC_DIR}/hash.cpp
        ${LIBCXX_SRC_DIR}/optional.cpp
        ${LIBCXX_SRC_DIR}/valarray.cpp
        ${LIBCXX_SRC_DIR}/system_error.cpp
)
target_compile_definitions(System.C++ PUBLIC ${FLAGS})
target_include_directories(System.C++ PUBLIC ${LIBCXX_INC_DIR})
target_compile_options(System.C++ PRIVATE -w)

add_library(System.C++ABI OBJECT
        # C++ABI files
        ${LIBCXXABI_SRC_DIR}/cxa_aux_runtime.cpp
#        ${LIBCXXABI_SRC_DIR}/cxa_default_handlers.cpp
        ${LIBCXXABI_SRC_DIR}/cxa_demangle.cpp
#        ${LIBCXXABI_SRC_DIR}/cxa_exception_storage.cpp
        ${LIBCXXABI_SRC_DIR}/cxa_guard.cpp
#        ${LIBCXXABI_SRC_DIR}/cxa_handlers.cpp
#        ${LIBCXXABI_SRC_DIR}/cxa_unexpected.cpp
        ${LIBCXXABI_SRC_DIR}/cxa_vector.cpp
        ${LIBCXXABI_SRC_DIR}/cxa_virtual.cpp
#        ${LIBCXXABI_SRC_DIR}/cxa_noexception.cpp

        # C++ STL files
        ${LIBCXXABI_SRC_DIR}/stdlib_exception.cpp
        ${LIBCXXABI_SRC_DIR}/stdlib_stdexcept.cpp
        ${LIBCXXABI_SRC_DIR}/stdlib_typeinfo.cpp

        # Internal files
        ${LIBCXXABI_SRC_DIR}/abort_message.cpp
        ${LIBCXXABI_SRC_DIR}/fallback_malloc.cpp
        ${LIBCXXABI_SRC_DIR}/private_typeinfo.cpp
)
target_compile_definitions(System.C++ABI PUBLIC ${FLAGS})
target_include_directories(System.C++ABI PUBLIC ${LIBCXX_INC_DIR} ${LIBCXXABI_INC_DIR})
target_compile_options(System.C++ABI PRIVATE -w)
