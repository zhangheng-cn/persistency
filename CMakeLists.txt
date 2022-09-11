cmake_minimum_required(VERSION 2.8)
project(persistency)

include_directories(
    .
    ara/core
    ara/per
    third_party/include
)

aux_source_directory(ara/core SRC_LIST)
aux_source_directory(ara/per SRC_LIST)


add_library(persistency STATIC ${SRC_LIST})

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/build
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib/leveldb
)


add_subdirectory(test)