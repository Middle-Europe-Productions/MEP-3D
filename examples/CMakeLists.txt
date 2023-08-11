set(MEP_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/output")

macro(mep_set_output_dir target)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${MEP_OUTPUT_DIR}/${target}")
endmacro()

macro(mep_project name)
  project(${name})
  mep_set_output_dir(${name})
endmacro()

macro(add_mep_executable project_name)
  set(targets)
  foreach(arg ${ARGV})
    if(NOT "${arg}" STREQUAL "${project_name}")
      list(APPEND targets ${arg})
    endif()
  endforeach()

  if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (WIN32)
      add_executable(${project_name} ${targets})
      target_link_options(${project_name} PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
    elseif(APPLE)
      add_executable(${project_name} MACOSX_BUNDLE ${targets})
    endif()
  else()
    add_executable(${project_name} ${targets})
  endif()
endmacro()

macro(copy_mep_resource resource resource_path)
message(STATUS ${resource} ${resource_path})
if (CMAKE_BUILD_TYPE STREQUAL "Release" AND APPLE)
foreach (FILE ${resource})
    file(RELATIVE_PATH NEW_FILE "${PROJECT_SOURCE_DIR}/${resource_path}/" ${FILE})
    get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
    set_property(SOURCE ${FILE} PROPERTY MACOSX_PACKAGE_LOCATION Resources/${resource_path}/${NEW_FILE_PATH})
    source_group(Resources/${NEW_FILE_PATH} FILES "${FILE}")
endforeach ()
else()
file(
  COPY
  ${resource}
  DESTINATION
  ${MEP_OUTPUT_DIR}/${PROJECT_NAME}/${resource_path}
)
endif()
endmacro()

if (NOT (MEP_TARGET_PROJECT STREQUAL "NONE"))
if (MEP_TARGET_PROJECT STREQUAL "ALL")
  message(STATUS "This project supports only the volume rendering target")
endif()
# Release targets
if (MEP_TARGET_PROJECT STREQUAL "volume-rendering")
add_subdirectory(volume_rendering)
endif()
endif()

if (MSVC)
file(
  COPY
  ${PROJECT_SOURCE_DIR}/scripts/run_mep_msvc.bat
  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}/
)
elseif(UNIX)
file(
  COPY
  ${PROJECT_SOURCE_DIR}/scripts/run_mep_unix.sh
  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}/
)
else()
    message(STATUS "Could not find build script for the compiler")
endif()
