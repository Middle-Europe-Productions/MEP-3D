mep_project(imgui_example)

set(PS_DIR ${PROJECT_SOURCE_DIR}/../../)

source_group("Shader Files")

add_mep_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)

file(
  COPY
  ${PS_DIR}/shaders
  DESTINATION
  ${MEP_OUTPUT_DIR}/imgui_example
)

file(
  COPY
  ${PROJECT_SOURCE_DIR}/textures
  DESTINATION
  ${MEP_OUTPUT_DIR}/imgui_example
)

file(
  COPY
  ${PROJECT_SOURCE_DIR}/models
  DESTINATION
  ${MEP_OUTPUT_DIR}/imgui_example
)

file(
  COPY
  ${PS_DIR}/runtime_configurations
  DESTINATION
  ${MEP_OUTPUT_DIR}/imgui_example
)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY 
  ${MEP_OUTPUT_DIR}
)
