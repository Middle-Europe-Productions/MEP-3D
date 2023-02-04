set(MEP_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/output")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/output")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/output")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/output")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/output")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/output")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/output")

macro(mep_set_output_dir target)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${MEP_OUTPUT_DIR}/${target}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${MEP_OUTPUT_DIR}/${target}")
endmacro()

add_subdirectory(imgui_example)
add_subdirectory(simple_example/)
add_subdirectory(imgui_custom_scene/)
add_subdirectory(camera_example/)
add_subdirectory(shader_example/)
add_subdirectory(volume_rendering)

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
