cmake_minimum_required(VERSION 3.10)
project(cpp-demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# set of required *compiled* boost libs
set(BOOST_LIBS_REQUIRED
    regex
    program_options
)

set(BOOST_LIBS_REQUIRED_TARGETS ${BOOST_LIBS_REQUIRED})
list(TRANSFORM BOOST_LIBS_REQUIRED_TARGETS PREPEND "Boost::")

option(DISABLE_BOOST "Don't build boost examples" No)

find_package(Threads)
find_package(Boost 1.78.0
             COMPONENTS ${BOOST_LIBS_REQUIRED})

file(GLOB SRC *.cpp)
file(GLOB BOOST_SRC boost/*.cpp)

if (DISABLE_BOOST)
    message("-- Boost examples are disabled...")
else()
    list(APPEND SRC ${BOOST_SRC})
endif()

message("Current: ${CMAKE_CURRENT_SOURCE_DIR}")

foreach(src_ ${SRC})
    get_filename_component(name_ ${src_} NAME_WE)

    set(target_libs_ ${CMAKE_THREAD_LIBS_INIT})

    if (${src_} IN_LIST BOOST_SRC)
        if (DISABLE_BOOST)
            continue()
        endif()
        list(APPEND target_libs_ ${BOOST_LIBS_REQUIRED_TARGETS})
    endif()

    add_executable(${name_} ${src_})
    target_link_libraries(${name_} ${target_libs_})
    target_include_directories(${name_} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

