cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cpp-demo)

file(GLOB SRC *.cpp)

foreach(src_ ${SRC})
    get_filename_component(name_ ${src_} NAME_WE)
    add_executable(${name_} ${src_})
endforeach()
