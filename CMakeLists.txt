cmake_minimum_required(VERSION 3.27)
project(spfa)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(spfa ${SRC_FILES})
set_target_properties(
	spfa PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)


set(graph_dir ${CMAKE_CURRENT_SOURCE_DIR}/..)
# target_include_directories(tester PRIVATE ${graph_dir}/preorder_traversal/include ${graph_dir}/splay_tree/include)

target_include_directories(spfa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(TESTING_MODE "auto")
if ("${TESTING_MODE}" STREQUAL "interactive")
	add_compile_definitions(INTERACTIVE)
endif()

target_include_directories(spfa PRIVATE ${CMAKE_CURRENT_BINARY_DIR})