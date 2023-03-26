mep_project(volume_rendering)

add_executable(${PROJECT_NAME} main.cpp
  transfer_function.hpp
  transfer_function.cpp
  point_canvas.hpp
  point_canvas.cpp
  volume_renderer.hpp
  volume_renderer.cpp
  common.hpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)

file(
  COPY
  ${PROJECT_SOURCE_DIR}/runtime_configurations
  DESTINATION
  ${MEP_OUTPUT_DIR}/${PROJECT_NAME}
)
