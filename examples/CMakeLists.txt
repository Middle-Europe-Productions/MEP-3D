add_subdirectory(imgui_example)
add_subdirectory(simple_example/)
add_subdirectory(imguI_custom_scene/)

if (MSVC)
file(
  COPY
  ${PROJECT_SOURCE_DIR}/scripts/run_mep_msvc.bat
  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}/
)
else()
    message(STATUS "Could not find build script for the compiler")
endif()
