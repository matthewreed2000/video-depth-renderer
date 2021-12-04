cmake_minimum_required(VERSION 3.14)
project(video-depth-renderer C CXX)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glew)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/ffmpeg)

if (APPLE)
	list(APPEND EXTRA_LIBS
		"-framework OpenGL"
	)
elseif(WIN32)
	list(APPEND EXTRA_LIBS
		"-lglew32 -lopengl32"
	)
	set(CMAKE_EXE_LINKER_FLAGS "-std=gnu99 -static -static-libgcc -static-libstdc++ -mwindows")
else()
	list(APPEND EXTRA_LIBS
		"-lGL -lGLEW -lX11"
	)
endif()

list(APPEND SOURCES
	${PROJECT_SOURCE_DIR}/src/main.cpp
)

add_executable(video-depth-renderer ${SOURCES})
target_link_libraries(video-depth-renderer glew glfw glm ffmpeg ${EXTRA_LIBS})