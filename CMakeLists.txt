project(Toutatis)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

set(CMAKE_AUTOMOC TRUE)

include(cmake/DBusMacros.cmake)

add_subdirectory(daemon)
# add_subdirectory(client)
add_subdirectory(git)
add_subdirectory(qtlib)