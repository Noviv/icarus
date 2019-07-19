project(client)

cmake_minimum_required(VERSION 3.5)

if(WITH_OGRE)
	add_subdirectory(ogre)
endif()

add_executable(client client.cc)
