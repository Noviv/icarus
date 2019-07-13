find_package(Threads)

add_executable(server server.cc)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})
