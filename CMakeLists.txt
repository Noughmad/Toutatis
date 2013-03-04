project(Toutatis)
cmake_minimum_required(VERSION 2.8)

find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Xml REQUIRED)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(QT_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5DBus_INCLUDE_DIRS} ${Qt5Sql_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})

include(cmake/DBusMacros.cmake)

add_subdirectory(daemon)
add_subdirectory(client)
add_subdirectory(git)
add_subdirectory(qtlib)
add_subdirectory(timesync)
