include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(Toutatis_qtlib_SRCS
    toutatis.cpp
    project.cpp
    task.cpp
    event.cpp
    note.cpp
   # tasktreemodel.cpp
)

qt5_add_dbus_interface(Toutatis_qtlib_SRCS ../com.noughmad.Toutatis.xml toutatis_interface)
qt5_add_dbus_interface(Toutatis_qtlib_SRCS ../com.noughmad.toutatis.Project.xml project_interface)
qt5_add_dbus_interface(Toutatis_qtlib_SRCS ../com.noughmad.toutatis.Task.xml task_interface)
qt5_add_dbus_interface(Toutatis_qtlib_SRCS ../com.noughmad.toutatis.Event.xml event_interface)
qt5_add_dbus_interface(Toutatis_qtlib_SRCS ../com.noughmad.toutatis.Note.xml note_interface)

add_library(qtatis SHARED ${Toutatis_qtlib_SRCS})
target_link_libraries(qtatis ${Qt5Core_LIBRARIES} ${Qt5DBus_LIBRARIES})

install(TARGETS qtatis RUNTIME DESTINATION bin LIBRARY DESTINATION lib)


