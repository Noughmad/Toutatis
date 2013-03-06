set(ToutatisQml_SRCS
    toutatisplugin.cpp
)

include_directories(
    ${QT_INCLUDES}
    ../qtlib/
)

add_library(qmltoutatisplugin MODULE ${ToutatisQml_SRCS})
target_link_libraries(qmltoutatisplugin
    qtatis
)
install(TARGETS qmltoutatisplugin DESTINATION share/Toutatis)
install(FILES qmldir DESTINATION share/Toutatis)

