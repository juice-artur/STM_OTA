if(DEFINED ASTRA_UTEST_INCLUDED)
    return()
endif()
set(ASTRA_UTEST_INCLUDED TRUE)

set(_ASTRA_UTEST_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/utest.h")

if(NOT EXISTS "${_ASTRA_UTEST_INCLUDE_DIR}/utest.h")
    message(FATAL_ERROR
            "utest.h not found in ${_ASTRA_UTEST_INCLUDE_DIR}")
endif()

add_library(utest INTERFACE)
target_include_directories(utest SYSTEM INTERFACE
                           "${_ASTRA_UTEST_INCLUDE_DIR}")
add_library(utest::utest ALIAS utest)