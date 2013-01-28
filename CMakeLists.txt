project(Toutatis)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(Toutatis_SRCS toutatis.cpp main.cpp)
qt4_automoc(${Toutatis_SRCS})
add_executable(Toutatis ${Toutatis_SRCS})
target_link_libraries(Toutatis ${QT_QTCORE_LIBRARY})

install(TARGETS Toutatis RUNTIME DESTINATION bin)
