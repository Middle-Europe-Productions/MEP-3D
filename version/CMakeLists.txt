find_package(Git)

set(MEP_VERSION_HEADER_IN ${CMAKE_SOURCE_DIR}/version/version.hpp.in)

if(GIT_EXECUTABLE)
    get_filename_component(MEP_VERSION_HEADER_IN_DIR ${MEP_VERSION_HEADER_IN} DIRECTORY)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        WORKING_DIRECTORY ${MEP_VERSION_HEADER_IN_DIR}
        OUTPUT_VARIABLE GIT_DESCRIBE_VERSION
        RESULT_VARIABLE GIT_DESCRIBE_ERROR_CODE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+).?(.*)?" match_result "${GIT_DESCRIBE_VERSION}")
    set(major "${CMAKE_MATCH_1}")
    set(minor "${CMAKE_MATCH_2}")
    set(patch "${CMAKE_MATCH_3}")
    set(suffix "${CMAKE_MATCH_4}")

    if(NOT GIT_DESCRIBE_ERROR_CODE)
        set(MEP_VERSION ${GIT_DESCRIBE_VERSION})
    endif()
    message("[MEP] Detected version: ${major}.${minor}.${patch}.${suffix}")
endif()

if(NOT DEFINED MEP_VERSION)
    set(MEP_VERSION 0.0.0)
    set(major "0")
    set(minor "0")
    set(patch "0")
    set(suffix "")
    message(WARNING "Failed to determine MEP_VERSION from Git tags. Using default version \"${MEP_VERSION}\".")
endif()

configure_file(${MEP_VERSION_HEADER_IN} ${CMAKE_SOURCE_DIR}/include/MEP-3D/version.hpp @ONLY)