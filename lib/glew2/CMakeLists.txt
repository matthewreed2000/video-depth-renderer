cmake_minimum_required(VERSION 3.14)
project(glew)

add_library(glew INTERFACE IMPORTED GLOBAL)
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(glew INTERFACE ${GLEW_LIBRARIES})