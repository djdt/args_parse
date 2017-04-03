cmake_minimum_required(VERSION 3.5)
project(ArgsParse)

set(CMAKE_BUILD_TYPE Debug)

include_directories(
	${ArgsParse_SOURCE_DIR}/include
)

file(GLOB_RECURSE sources_ArgsParse ${ArgsParse_SOURCE_DIR}/src/*.cpp)

add_library(ArgsParse ${sources_ArgsParse})

target_link_libraries(ArgsParse
)
