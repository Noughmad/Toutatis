include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(Toutatis_client_SRCS ../qtlib/tasktreemodel.cpp
    main.cpp
    client.cpp
)

qt4_automoc(${Toutatis_client_SRCS})

add_executable(toutatis ${Toutatis_client_SRCS})
target_link_libraries(toutatis ${QT_QTCORE_LIBRARY} ${QT_QTSQL_LIBRARY} ${QT_QTDBUS_LIBRARY})

install(TARGETS toutatis RUNTIME DESTINATION bin)

