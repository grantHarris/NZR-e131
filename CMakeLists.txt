cmake_minimum_required(VERSION 3.7)

set(MY_TARGET_NAME "e131bridge")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -DBOOST_LOG_DYN_LINK")

project(${MY_TARGET_NAME})

file(GLOB SOURCES "src/*.cpp")

link_directories("lib/")

find_library(BOOST_LOG boost_log)
find_library(BOOST_PROG_OPT boost_program_options)
find_library(YAML_CPP_LIBRARY yaml-cpp)
find_library(E131 e131)

add_executable(${MY_TARGET_NAME} ${SOURCES})

target_link_libraries(${MY_TARGET_NAME} ${boost_log} ${boost_program_options} ${YAML_CPP_LIBRARY} ${E131})
target_link_libraries(${MY_TARGET_NAME} ${CMAKE_SOURCE_DIR}/lib/libws2811.a)