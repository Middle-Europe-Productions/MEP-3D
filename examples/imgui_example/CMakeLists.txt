project(imgui_example)

set(SHADERS
  ${PROJECT_SOURCE_DIR}/shaders/shader.frag
  ${PROJECT_SOURCE_DIR}/shaders/shader.vert
)

set(RESOURCE_DIR
  ${PROJECT_SOURCE_DIR}/shaders
  ${PROJECT_SOURCE_DIR}/textures
)

source_group("Shader Files" FILES ${SHADERS})

add_executable(${PROJECT_NAME} main.cpp ${SHADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)

file(
  COPY
  ${PROJECT_SOURCE_DIR}/shaders
  DESTINATION
  ${PROJECT_BINARY_DIR}/
)

file(
  COPY
  ${PROJECT_SOURCE_DIR}/textures
  DESTINATION
  ${PROJECT_BINARY_DIR}/
)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY 
  ${PROJECT_BINARY_DIR}
)
