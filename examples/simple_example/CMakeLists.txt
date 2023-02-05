mep_project(simple_example)

set(SHADERS_DIR ${PROJECT_SOURCE_DIR}/../../shaders)

source_group("Shader Files")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)

file(
  COPY
  ${SHADERS_DIR}/
  DESTINATION
  ${MEP_OUTPUT_DIR}/simple_example
)

file(
  COPY
  ${PROJECT_SOURCE_DIR}/textures
  DESTINATION
  ${MEP_OUTPUT_DIR}/simple_example
)

file(
  COPY
  ${PROJECT_SOURCE_DIR}/models
  DESTINATION
  ${MEP_OUTPUT_DIR}/simple_example
)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY 
  ${PROJECT_BINARY_DIR}
)
