set(ToutatisQml_SRCS
    toutatisplugin.cpp
)

include_directories(
    ${QT_INCLUDES}
    ../qtlib/
    ${CMAKE_CURRENT_BINARY_DIR}/../qtlib/
)

add_library(qmltoutatisplugin MODULE ${ToutatisQml_SRCS})
target_link_libraries(qmltoutatisplugin
    qtatis
    ${QT_QTDECLARATIVE_LIBRARY}
)
install(TARGETS qmltoutatisplugin DESTINATION ${IMPORTS_INSTALL_DIR}/com/noughmad/toutatis)
install(FILES qmldir DESTINATION ${IMPORTS_INSTALL_DIR}/com/noughmad/toutatis)

