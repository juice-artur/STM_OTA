cmake_minimum_required(VERSION 3.22)
project(AstraProtocol LANGUAGES C)

include("${CMAKE_CURRENT_LIST_DIR}/AstraProtocol.cmake")

option(ASTRA_BUILD_TESTS "Build Astra Protocol unit tests" ON)

if(ASTRA_BUILD_TESTS)
    include("${CMAKE_CURRENT_LIST_DIR}/Tests/AstraProtocolTests.cmake")
endif()