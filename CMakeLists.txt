cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cpp-demo)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package (Threads)

# set(BOOST_URL https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.7z)
# set(BOOST_URL_SHA256 090cefea470bca990fa3f3ed793d865389426915b37a2a3258524a7258f0790c)
add_subdirectory(boost-cmake)

file(GLOB SRC *.cpp)

foreach(src_ ${SRC})
    get_filename_component(name_ ${src_} NAME_WE)
    add_executable(${name_} ${src_})
    target_link_libraries (${name_} ${CMAKE_THREAD_LIBS_INIT} )

    if (${name_} MATCHES "boost-.*")
        target_link_libraries(${name_} Boost::boost Boost::regex Boost::program_options)
    endif()
endforeach()

