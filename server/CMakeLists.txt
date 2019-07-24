project(server)

cmake_minimum_required(VERSION 3.5)

include(../defaults.cmake)

find_package(Threads)

include_directories(.)

add_executable(server server.cc)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})
