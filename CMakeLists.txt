cmake_minimum_required(VERSION 3.20)
project(BitonicSort LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(driver
    src/driver.cpp
)

target_include_directories(driver PRIVATE
    include
)
