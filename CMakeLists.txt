cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cpp-demo)

find_package (Threads)

set(BOOST_URL https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.7z)
set(BOOST_URL_SHA256 e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855)
add_subdirectory(boost-cmake)

file(GLOB SRC *.cpp)

foreach(src_ ${SRC})
    get_filename_component(name_ ${src_} NAME_WE)
    add_executable(${name_} ${src_})
    target_link_libraries (${name_} ${CMAKE_THREAD_LIBS_INIT} )

    if (${name_} MATCHES "boost-.*")
        target_link_libraries(${name_} Boost::boost Boost::regex)
    endif()
endforeach()
