cmake_minimum_required(VERSION 3.0.0)

set(SOURCES 
	btree.cpp
	  ${CMAKE_SOURCE_DIR}/include/btree/btree.h)

add_library(btree ${SOURCES})

target_include_directories(btree PRIVATE ${CMAKE_SOURCE_DIR}/include/btree)

install(TARGETS btree 
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
