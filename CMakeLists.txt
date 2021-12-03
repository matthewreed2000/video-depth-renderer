cmake_minimum_required(VERSION 3.14)
project(video-depth-renderer C CXX)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/glfw)
add_subdirectory(lib/ffmpeg)

if (APPLE)
	list(APPEND EXTRA_LIBS
		"-framework OpenGL"
	)
elseif(WIN32)
	list(APPEND EXTRA_LIBS
		"-lglu32 -lopengl32"
	)
	set(CMAKE_EXE_LINKER_FLAGS "-std=gnu99 -static -static-libgcc -static-libstdc++ -mwindows")
else()
	list(APPEND EXTRA_LIBS
		"-lGL -lGLU -lX11"
	)
endif()

list(APPEND SOURCES
	src/main.cpp
)

add_executable(video-depth-renderer ${SOURCES})
target_link_libraries(video-depth-renderer ffmpeg glfw ${EXTRA_LIBS})