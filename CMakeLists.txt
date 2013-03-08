project(Toutatis)
cmake_minimum_required(VERSION 2.8)

find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)
include(KDE4Defaults)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

include(cmake/DBusMacros.cmake)

# add_subdirectory(daemon)
add_subdirectory(client)
add_subdirectory(git)
add_subdirectory(qtlib)
add_subdirectory(dataengine)
add_subdirectory(plasmoid)
add_subdirectory(qml)