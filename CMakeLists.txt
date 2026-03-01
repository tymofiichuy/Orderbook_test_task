cmake_minimum_required(VERSION 3.10)
project(orderbook)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(MAIN_DIR ${CMAKE_SOURCE_DIR}/main)
# set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)

set(SOURCES
    ${SOURCE_DIR}/main_logic.cpp
    ${SOURCE_DIR}/cli.cpp
)

include_directories(${SOURCE_DIR})

add_executable(orderbook ${MAIN_DIR}/driver.cpp ${SOURCES})
target_include_directories(orderbook PUBLIC ${SOURCE_DIR})