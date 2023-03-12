mep_project(volume_rendering)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)

file(
  COPY
  ${PROJECT_SOURCE_DIR}/runtime_configurations
  DESTINATION
  ${MEP_OUTPUT_DIR}/${PROJECT_NAME}
)
