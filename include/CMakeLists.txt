#We need to source the include files, so that QT's MOC can see them.
target_sources(appRedundantCoding PRIVATE
    ${INCLUDE_FILES}
)
