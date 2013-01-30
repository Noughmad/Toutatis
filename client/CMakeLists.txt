include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(Toutatis_client_SRCS
    main.cpp
)

qt4_automoc(${Toutatis_client_SRCS})

qt4_add_dbus_interface(Toutatis_client_SRCS ../com.noughmad.Toutatis.xml toutatis_interface)
qt4_add_dbus_interface(Toutatis_client_SRCS ../com.noughmad.Toutatis.Project.xml project_interface)
qt4_add_dbus_interface(Toutatis_client_SRCS ../com.noughmad.Toutatis.Task.xml task_interface)

add_executable(toutatis ${Toutatis_client_SRCS})
target_link_libraries(toutatis ${QT_QTCORE_LIBRARY} ${QT_QTSQL_LIBRARY} ${QT_QTDBUS_LIBRARY})

install(TARGETS toutatis RUNTIME DESTINATION bin)

