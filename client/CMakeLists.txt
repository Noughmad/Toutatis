include_directories(
    ${QT_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../qtlib
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/../qtlib
)

set(Toutatis_client_SRCS
    main.cpp
    client.cpp
)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

add_executable(toutatis ${Toutatis_client_SRCS})
target_link_libraries(toutatis qtatis ${QT_QTCORE_LIBRARY})

install(TARGETS toutatis RUNTIME DESTINATION bin)

