cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cpp-demo)

find_package (Threads)

find_package(Boost COMPONENTS regex)

file(GLOB SRC *.cpp)

foreach(src_ ${SRC})
    get_filename_component(name_ ${src_} NAME_WE)
    add_executable(${name_} ${src_})
    target_link_libraries (${name_} ${CMAKE_THREAD_LIBS_INIT} )

    if (${name_} MATCHES "boost-.*")
        target_link_libraries(${name_} ${Boost_LIBRARIES})
    endif()
endforeach()
