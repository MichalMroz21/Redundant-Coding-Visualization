file(GLOB FOLDER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

target_sources(appRedundantCoding PRIVATE
    ${FOLDER_FILES}
)
