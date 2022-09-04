project(imgui_example)

set(SHADERS_DIR ${PROJECT_SOURCE_DIR}/../../shaders)

set(SHADERS
  ${SHADERS_DIR}/shader.frag
  ${SHADERS_DIR}/shader.vert
)

source_group("Shader Files" FILES ${SHADERS})

add_executable(${PROJECT_NAME} main.cpp ${SHADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)

file(
  COPY
  ${SHADERS_DIR}/
  DESTINATION
  ${PROJECT_BINARY_DIR}/shaders
)

file(
  COPY
  ${PROJECT_SOURCE_DIR}/textures
  DESTINATION
  ${PROJECT_BINARY_DIR}/
)

file(
  COPY
  ${PROJECT_SOURCE_DIR}/models
  DESTINATION
  ${PROJECT_BINARY_DIR}/
)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY 
  ${PROJECT_BINARY_DIR}
)
