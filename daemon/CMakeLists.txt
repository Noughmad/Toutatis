include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(Toutatis_daemon_SRCS
    utils.cpp
    model.cpp
    task.cpp
    project.cpp
    toutatis.cpp
    main.cpp
)

qt4_automoc(${Toutatis_daemon_SRCS})
qt4_add_dbus_adaptor(Toutatis_daemon_SRCS ../com.noughmad.Toutatis.xml toutatis.h Toutatis)
qt4_add_dbus_adaptor(Toutatis_daemon_SRCS ../com.noughmad.toutatis.Project.xml project.h Project)
qt4_add_dbus_adaptor(Toutatis_daemon_SRCS ../com.noughmad.toutatis.Task.xml task.h Task)
dbus_add_activation_service(com.noughmad.Toutatis.service.in)

add_executable(toutatisd ${Toutatis_daemon_SRCS})
target_link_libraries(toutatisd ${QT_QTCORE_LIBRARY} ${QT_QTSQL_LIBRARY} ${QT_QTDBUS_LIBRARY})

install(TARGETS toutatisd RUNTIME DESTINATION bin)
