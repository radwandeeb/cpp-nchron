cmake_minimum_required(VERSION 2.6)

set(PROTOCOL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROTOCOL_INCLUDE_DIR ${PROTOCOL_INCLUDE_DIR} PARENT_SCOPE)

if(TARGET protocol)
	message("protocol is already defined")
else()
	include_directories(${PROTOCOL_INCLUDE_DIR})
	set(LIB_SRCS protocol.h protocol_structures.h protocol.cpp protocol_commands.cpp)
	add_library(protocol STATIC ${LIB_SRCS})
endif()