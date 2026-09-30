cmake_minimum_required(VERSION 3.22)

add_library(AstraCrc32 STATIC "${CMAKE_CURRENT_LIST_DIR}/Src/AstraCrc32.c")
target_sources(AstraCrc32 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Include/AstraCrc32.h")

target_include_directories(AstraCrc32 PUBLIC "${CMAKE_CURRENT_LIST_DIR}/Include")