cmake_minimum_required(VERSION 3.0.0)

add_executable(btreetests btreetests.cpp)
include_directories(${CMAKE_SOURCE_DIR}/include/btree)
target_link_libraries(btreetests btree gtest gtest_main)
install(TARGETS btreetests RUNTIME DESTINATION bin)

add_test(
	NAME btreetests
	COMMAND btreetests
)
