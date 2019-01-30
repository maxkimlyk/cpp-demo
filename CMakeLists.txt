cmake_minimum_required(VERSION 3.10)

project(cpp-demo)

file(GLOB SRC *.cpp)

foreach(src_ ${SRC})
    get_filename_component(name_ ${src_} NAME_WE)
    add_executable(${name_} ${src_})
endforeach()
