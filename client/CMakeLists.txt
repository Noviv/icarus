project(client)

cmake_minimum_required(VERSION 3.5)

add_subdirectory(ogre)

add_executable(client client.cc)
