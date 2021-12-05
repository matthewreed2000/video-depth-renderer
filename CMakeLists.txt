# Based on: https://shot511.github.io/2018-05-29-how-to-setup-opengl-project-with-cmake/
cmake_minimum_required(VERSION 3.14)
project(video-depth-renderer C CXX)

# Add source files
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp
	${CMAKE_SOURCE_DIR}/lib/glad/src/glad.c
)

# Add header files
file(GLOB_RECURSE HEADER_FILES
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp
)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# External Libraries
find_package(OpenGL REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/ffmpeg)

add_library(GLAD "${CMAKE_SOURCE_DIR}/lib/glad/src/glad.c")

set(LIBS glfw OpenGL::GL GLAD)

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/lib/glad/include"
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})