cmake_minimum_required(VERSION 3.12)
project(Elect)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src/test)
set(SRC_CONFIG
        src/config/config.h
        src/config/config.cc)
