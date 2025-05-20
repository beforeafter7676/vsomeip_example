cmake_minimum_required(VERSION 3.5)
project(vsomeip_example)

set(CMAKE_CXX_STANDARD 14)

find_package(vsomeip REQUIRED)

add_executable(server server.cpp)
add_executable(client client.cpp)

target_link_libraries(server vsomeip)
target_link_libraries(client vsomeip)
