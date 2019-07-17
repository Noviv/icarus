project(client)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(ogre)

add_executable(client client.cc)
