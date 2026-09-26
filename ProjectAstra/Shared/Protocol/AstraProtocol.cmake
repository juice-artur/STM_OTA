cmake_minimum_required(VERSION 3.22)

add_library(AstraProtocol STATIC "${CMAKE_CURRENT_LIST_DIR}/Src/AstraCobs.c")
target_sources(AstraProtocol PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Include/AstraCobs.h"
                                     "${CMAKE_CURRENT_LIST_DIR}/Include/AstraCobsEnums.h"
)

target_include_directories(AstraProtocol PUBLIC "${CMAKE_CURRENT_LIST_DIR}/Include")
