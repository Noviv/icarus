project(icarus)

cmake_minimum_required(VERSION 3.5)

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

add_executable(server server.cc)
add_executable(client client.cc)
