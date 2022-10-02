add_subdirectory(imgui_example)
add_subdirectory(simple_example/)
add_subdirectory(imgui_custom_scene/)
add_subdirectory(camera_example/)

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
