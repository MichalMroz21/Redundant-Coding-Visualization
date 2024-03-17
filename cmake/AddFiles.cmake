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
