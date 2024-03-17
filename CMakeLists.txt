cmake_minimum_required(VERSION 3.24)

project(RedundantCoding VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

#Some libraries are header-only and some require building, this separation is required as boost is added both with find_package and submodules
#As described here: https://www.boost.org/doc/libs/1_84_0/more/getting_started/windows.html#header-only-libraries
set(BOOST_HEADER_ONLY_LIBRARIES algorithm)
set(BOOST_BUILD_LIBRARIES filesystem)
set(BOOST_ENABLE_CMAKE ON)

set(QT_LIBRARIES Quick Concurrent)

option(MAKE_TEST_EXE "Run GTests" ON)

#CMakeConfig.hpp - constants from cmake for the entire project to use
add_definitions(-DROOT_PATH_CMAKE=${CMAKE_CURRENT_SOURCE_DIR})

#Macros
include(${CMAKE_DIR}/UpdateSubmodules.cmake)
include(${CMAKE_DIR}/AddFiles.cmake)

find_package(Qt6 6.4 REQUIRED COMPONENTS ${QT_LIBRARIES})

#Boost and GTest can be downloaded via git submodules
find_package(Boost COMPONENTS ${BOOST_BUILD_LIBRARIES})
find_package(GTest)

add_subdirectory(dependencies)

#Create executable
qt_standard_project_setup()
qt_add_executable(appRedundantCoding)
qt_policy(SET QTP0001 NEW)

#Get cpp files to add to Executables
file(GLOB_RECURSE SRC_FILES ${SOURCE_DIR}/*.cpp)

#Get include files to add to Executables (Required for MOC)
file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE_DIR}/*.hpp)

add_subdirectory(source)
add_subdirectory(include)
add_subdirectory(source_gui)
add_subdirectory(assets)

target_include_directories(appRedundantCoding PRIVATE
    ${INCLUDE_DIR}
)

#Transform library names into proper form for linking
list(TRANSFORM BOOST_BUILD_LIBRARIES PREPEND "Boost::")
list(TRANSFORM BOOST_HEADER_ONLY_LIBRARIES PREPEND "Boost::")
list(TRANSFORM QT_LIBRARIES PREPEND "Qt6::")

#Combine 2 lists into one
list(APPEND BOOST_LIBRARIES ${BOOST_BUILD_LIBRARIES})
list(APPEND BOOST_LIBRARIES ${BOOST_HEADER_ONLY_LIBRARIES})

#Link to main application
target_link_libraries(appRedundantCoding
    PRIVATE ${BOOST_LIBRARIES} ${QT_LIBRARIES}
)

qt_add_qml_module(appRedundantCoding
    URI RedundantCoding
    VERSION 1.0
    RESOURCE_PREFIX "/"

    QML_FILES ${QML_FILES}
    RESOURCES ${RESOURCES}
)

#Create separate Tester executables that runs tests
if(MAKE_TEST_EXE)
    include(CTest)
    add_subdirectory(tests)
endif()

set_target_properties(appRedundantCoding PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS appRedundantCoding
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
