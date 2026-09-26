cmake_minimum_required(VERSION 3.22)

add_library(AstraOrotocol STATIC my_math.cpp)

target_include_directories(AstraOrotocol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)