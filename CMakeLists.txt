cmake_minimum_required(VERSION 3.10)

project(e131bridge)

include_directories(lib)

file(GLOB SOURCES "src/*.cpp")

find_library(BOOST_LIBRARY Boost)
find_library(YAML_CPP_LIBRARY yaml-cpp)
find_library(E131 e131)

add_executable(e131bridge ${SOURCES})

target_link_libraries(e131bridge ${BOOST_LIBRARY} ${YAML_CPP_LIBRARY} ${E131})