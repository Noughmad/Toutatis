include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(Toutatis_daemon_SRCS 
    model.cpp
    event.cpp
    task.cpp
    project.cpp
    note.cpp

    utils.cpp
    toutatis.cpp
    main.cpp
)

qt5_add_dbus_adaptor(Toutatis_daemon_SRCS ../com.noughmad.Toutatis.xml toutatis.h Toutatis)
qt5_add_dbus_adaptor(Toutatis_daemon_SRCS ../com.noughmad.toutatis.Project.xml project.h Project)
qt5_add_dbus_adaptor(Toutatis_daemon_SRCS ../com.noughmad.toutatis.Task.xml task.h Task)
qt5_add_dbus_adaptor(Toutatis_daemon_SRCS ../com.noughmad.toutatis.Event.xml event.h Event)
qt5_add_dbus_adaptor(Toutatis_daemon_SRCS ../com.noughmad.toutatis.Note.xml note.h Note)
dbus_add_activation_service(com.noughmad.Toutatis.service.in)

add_executable(toutatisd ${Toutatis_daemon_SRCS})
target_link_libraries(toutatisd ${Qt5Core_LIBRARIES} ${Qt5DBus_LIBRARIES} ${Qt5Sql_LIBRARIES} ${Qt5Network_LIBRARIES})

install(TARGETS toutatisd RUNTIME DESTINATION bin)
