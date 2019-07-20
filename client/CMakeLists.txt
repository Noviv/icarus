project(client)

cmake_minimum_required(VERSION 3.5)

include(../defaults.cmake)

if(POLICY CMP0072)
	cmake_policy(SET CMP0072 NEW)
endif()
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(client client.cc)
target_link_libraries(client glfw ${OPENGL_gl_LIBRARY})
target_include_directories(client PUBLIC ${OPENGL_INCLUDE_DIR})
