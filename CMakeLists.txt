cmake_minimum_required(VERSION 3.10)

project(e131bridge)

#Bring the headers, such as Student.h into the project
include_directories(lib)

file(GLOB SOURCES "src/*.cpp")
 
add_executable(e131bridge ${SOURCES})