mep_project(volume_rendering)

file(
  GLOB_RECURSE RC "${PROJECT_SOURCE_DIR}/runtime_configurations/*"
)

file(
  GLOB_RECURSE SHADERS "${PROJECT_SOURCE_DIR}/shaders/*"
)

file(
  GLOB_RECURSE DATA "${PROJECT_SOURCE_DIR}/data/*"
)

add_mep_executable(${PROJECT_NAME} main.cpp
  transfer_function.hpp
  transfer_function.cpp
  point_canvas.hpp
  point_canvas.cpp
  volume_renderer.hpp
  volume_renderer.cpp
  common.hpp
  ${RC}
  ${SHADERS}
)

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)

copy_mep_resource("${RC}" "runtime_configurations")

copy_mep_resource("${SHADERS}" "shaders")

copy_mep_resource("${DATA}" "")

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
  ${MEP_OUTPUT_DIR}/${PROJECT_NAME}
)
