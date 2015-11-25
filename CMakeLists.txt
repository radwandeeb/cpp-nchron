cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(docopt)
add_subdirectory(protocol)

add_subdirectory(nchron_src)
add_subdirectory(ndot_src)