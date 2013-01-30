project(Toutatis)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include(cmake/DBusMacros.cmake)

add_subdirectory(daemon)
add_subdirectory(client)