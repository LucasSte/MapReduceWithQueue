cmake_minimum_required(VERSION 3.5)
project(MapReduceWithQueue)

set(CMAKE_CXX_STANDARD 14)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.65.1 COMPONENTS filesystem)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(MapReduceWithQueue main.cpp Map.cpp Map.h)
    target_link_libraries(MapReduceWithQueue ${Boost_LIBRARIES})
endif()