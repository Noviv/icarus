project(icarus)

cmake_minimum_required(VERSION 3.5)

# add_subdirectory(ogre)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
