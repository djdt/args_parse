cmake_minimum_required(VERSION 3.5)
project(args_parse)

set(CMAKE_BUILD_TYPE Release)

include_directories(
	${${PROJECT_NAME}_SOURCE_DIR}/include
)

file(GLOB_RECURSE SRC_FILES ${${PROJECT_NAME}_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME}
)
