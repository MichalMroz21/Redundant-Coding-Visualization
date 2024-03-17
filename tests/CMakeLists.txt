file(GLOB_RECURSE TEST_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

qt_add_executable(Tester)

list(REMOVE_ITEM SRC_FILES ${SOURCE_DIR}/main.cpp)

#MOC requires to have include files sourced.
target_sources(Tester PRIVATE
    ${TEST_FILES} ${SRC_FILES} ${INCLUDE_FILES}
)

target_include_directories(Tester PUBLIC ${INCLUDE_DIR})

if(${Boost_FOUND})
    target_include_directories(Tester PRIVATE ${Boost_INCLUDE_DIRS})
endif()

target_link_libraries(Tester PRIVATE ${GTEST_LINK} ${BOOST_LIBRARIES} ${QT_LIBRARIES})

include(GoogleTest)

#Discovery mode has to be PRE_TEST, because it won't work without it, yes.
gtest_discover_tests(Tester DISCOVERY_MODE PRE_TEST)
