include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(Toutatis_qtlib_SRCS
    toutatis.cpp
    project.cpp
    tasktreemodel.cpp
)

qt4_automoc(${Toutatis_qtlib_SRCS})

qt4_add_dbus_interface(Toutatis_qtlib_SRCS ../com.noughmad.Toutatis.xml toutatis_interface)
qt4_add_dbus_interface(Toutatis_qtlib_SRCS ../com.noughmad.toutatis.Project.xml project_interface)
qt4_add_dbus_interface(Toutatis_qtlib_SRCS ../com.noughmad.toutatis.Task.xml task_interface)

add_library(qtatis SHARED ${Toutatis_qtlib_SRCS})
target_link_libraries(qtatis ${QT_QTCORE_LIBRARY} ${QT_QTDBUS_LIBRARY})

install(TARGETS qtatis RUNTIME DESTINATION bin LIBRARY DESTINATION lib)


