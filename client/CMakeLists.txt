include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../qtlib ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../qtlib)

set(Toutatis_client_SRCS
    main.cpp
    client.cpp
)

add_executable(toutatis ${Toutatis_client_SRCS})
target_link_libraries(toutatis qtatis ${Qt5Core_LIBRARIES})

install(TARGETS toutatis RUNTIME DESTINATION bin)

