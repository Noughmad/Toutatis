install(DIRECTORY plasma-applet-toutatis/
        DESTINATION ${DATA_INSTALL_DIR}/plasma/plasmoids/com.noughmad.toutatis)
 
install(FILES plasma-applet-toutatis/metadata.desktop
        DESTINATION ${SERVICES_INSTALL_DIR} RENAME plasma-applet-toutatis.desktop)