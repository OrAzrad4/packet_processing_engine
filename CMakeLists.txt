cmake_minimum_required(VERSION 3.20)

project(packet_processing_engine LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(packet_engine
    src/ring_buffer.c
    src/packet_pool.c
)

target_include_directories(packet_engine
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

