project(icarus)

cmake_minimum_required(VERSION 3.5)

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

find_package(Threads)

add_executable(server server.cc)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})
add_executable(client client.cc)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})
