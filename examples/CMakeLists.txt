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


if (NOT (MEP_TARGET_PROJECT STREQUAL "NONE"))
if (MEP_TARGET_PROJECT STREQUAL "ALL")
add_subdirectory(imgui_example)
add_subdirectory(simple_example/)
add_subdirectory(imgui_custom_scene/)
add_subdirectory(camera_example/)
add_subdirectory(shader_example/)
add_subdirectory(volume_rendering)
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
