cmake_minimum_required (VERSION 2.6)
project (nchron)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(SOURCES
	src/main.cpp
	src/protocol.cpp
	src/protocol.h
	src/protocol_structures.h
	src/protocol_commands.cpp
)
 
# add the binary tree to the search path for include files
include_directories( ${PROJECT_BINARY_DIR} )
include_directories ("${PROJECT_SOURCE_DIR}/docopt")
add_subdirectory (docopt) 

# add the executable
add_executable (nchron ${SOURCES})
	target_link_libraries( 
	nchron 
	docopt
)