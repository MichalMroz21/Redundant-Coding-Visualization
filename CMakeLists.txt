cmake_minimum_required(VERSION 3.16)

macro(add_files file file_collection)

    file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

    if (_relPath)
        list (APPEND ${file_collection} "${_relPath}/${file}")
    else()
        list (APPEND ${file_collection} "${file}")
    endif()

    if (_relPath)
        set (${file_collection} ${${file_collection}} PARENT_SCOPE)
    endif()

endmacro()

project(RedundantCoding VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Concurrent)

qt_standard_project_setup()

qt_add_executable(appRedundantCoding)
qt_policy(SET QTP0001 NEW) #use new policy (old is deprecated)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(src_gui)
add_subdirectory(assets)
add_subdirectory(tests)

qt_add_qml_module(appRedundantCoding
    URI RedundantCoding
    VERSION 1.0
    RESOURCE_PREFIX "/"

    QML_FILES ${QML_FILES}
    RESOURCES ${RESOURCES}
)

#CMakeConfig.hpp - constants from cmake for the entire project to use
add_definitions(-DROOT_PATH_CMAKE=${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(appRedundantCoding PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(appRedundantCoding PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appRedundantCoding
    PRIVATE Qt6::Quick
    PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent
    PRIVATE ${GTEST_LIBRARIES}
)

install(TARGETS appRedundantCoding
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
