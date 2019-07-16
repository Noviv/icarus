project(client)

cmake_minimum_required(VERSION 3.5)

add_subdirectory(../common)

add_executable(client client.cc)
