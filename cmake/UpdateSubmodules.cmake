# Adapted from https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html

macro(update_submodule SUBMODULE_NAME)

    find_package(Git QUIET)

    if(GIT_FOUND)

        message(STATUS "Updating git submodule: ${SUBMODULE_NAME}")

        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --progress ${SUBMODULE_NAME}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE RETURN_CODE)

        if(NOT RETURN_CODE EQUAL "0")
            message(WARNING "Cannot update submodule. Git command failed with ${RETURN_CODE}")
            return()
        endif()

        message(STATUS "Git submodule - ${SUBMODULE_NAME} updated successfully")
    endif()

endmacro()