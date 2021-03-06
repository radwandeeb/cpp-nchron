cmake_minimum_required (VERSION 2.6)
project (nchron)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(SOURCES
	nchron.cpp
)

find_package(Boost COMPONENTS system REQUIRED)
include_directories( ${PROJECT_BINARY_DIR} ${Boost_INCLUDE_DIRS})
link_directories( ${Boost_LIBRARY_DIR} )
 
# add the binary tree to the search path for include files
include_directories(${PROJECT_BINARY_DIR})
include_directories (${DOCOPT_INCLUDE_DIR})
include_directories (${PROTOCOL_INCLUDE_DIR})
include_directories (${SERIAL_INCLUDE_DIR})

set(EXECUTABLE_OUTPUT_PATH ../)

# add the executable
add_executable (nchron ${SOURCES})
target_link_libraries( 
	nchron
	protocol
	docopt
	serial
	${Boost_LIBRARIES}
)