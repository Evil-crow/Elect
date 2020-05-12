cmake_minimum_required(VERSION 3.12)
project(Test)

set(CMAKE_CXX_STANDARD 14)

add_executable(config_test
        config_test.cc
        ${PROJECT_SOURCE_DIR}/../config/config.cc
        ${PROJECT_SOURCE_DIR}/../config/config.h)
target_link_libraries(config_test yaml-cpp)