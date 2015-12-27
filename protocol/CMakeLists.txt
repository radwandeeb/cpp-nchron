cmake_minimum_required(VERSION 2.6)

set(PROTOCOL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROTOCOL_INCLUDE_DIR ${PROTOCOL_INCLUDE_DIR} PARENT_SCOPE)

find_package(Boost COMPONENTS system REQUIRED)
include_directories( ${PROJECT_BINARY_DIR} ${Boost_INCLUDE_DIRS})

if(TARGET protocol)
	message("protocol is already defined")
else()
	include_directories(${SERIAL_INCLUDE_DIR})
	include_directories(${PROTOCOL_INCLUDE_DIR})
	set(LIB_SRCS protocol.h protocol_structures.h protocol.cpp protocol_commands.cpp)
	add_library(protocol STATIC ${LIB_SRCS})
endif()
