file(GLOB FOLDER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.qml)

foreach(QML_FILE ${FOLDER_FILES})
    add_files(${QML_FILE} "QML_FILES")
endforeach()
