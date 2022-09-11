cmake_minimum_required(VERSION 2.8)
project(test_per)

add_executable(test_per "test_per.cpp")

target_link_libraries(test_per -lpersistency -lleveldb)