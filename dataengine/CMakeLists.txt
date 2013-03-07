add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/../qtlib
    ${CMAKE_CURRENT_BINARY_DIR}/../qtlib
)
 
set(tasks_engine_SRCS tasksengine.cpp)
 
kde4_add_plugin(plasma_engine_toutatis ${tasks_engine_SRCS})
target_link_libraries(plasma_engine_toutatis qtatis ${KDE4_KDECORE_LIBS} ${KDE4_PLASMA_LIBS} ${QT_QTCORE_LIBRARY})
 
install(TARGETS plasma_engine_toutatis DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-toutatis.desktop DESTINATION ${SERVICES_INSTALL_DIR})