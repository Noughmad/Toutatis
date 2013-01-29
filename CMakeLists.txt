project(Toutatis)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(Toutatis_SRCS project.cpp toutatis.cpp main.cpp)

qt4_automoc(${Toutatis_SRCS})
qt4_add_dbus_adaptor(Toutatis_SRCS com.noughmad.Toutatis.xml toutatis.h Toutatis)
qt4_add_dbus_adaptor(Toutatis_SRCS com.noughmad.Toutatis.Project.xml project.h Project)

add_executable(Toutatis ${Toutatis_SRCS})
target_link_libraries(Toutatis ${QT_QTCORE_LIBRARY} ${QT_QTSQL_LIBRARY} ${QT_QTDBUS_LIBRARY})

install(TARGETS Toutatis RUNTIME DESTINATION bin)
