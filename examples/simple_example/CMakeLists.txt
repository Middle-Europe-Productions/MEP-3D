project(simple_example)

set(SHADERS
  ${CMAKE_SOURCE_DIR}/shaders/shader.frag
  ${CMAKE_SOURCE_DIR}/shaders/shader.vert
)

set(RESOURCE_DIR
  ${CMAKE_SOURCE_DIR}/shaders
  ${PROJECT_SOURCE_DIR}/textures
)

source_group("Shader Files" FILES ${SHADERS})

add_executable(${PROJECT_NAME} main.cpp ${SHADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)

configure_file(
  ${SHADERS}/shaders/shader.frag
  ${PROJECT_BINARY_DIR}/shader.frag
  COPYONLY
)
configure_file(
  ${SHADERS}/shaders/shader.vert
  ${PROJECT_BINARY_DIR}/shader.vert
  COPYONLY
)
configure_file(
  ${PROJECT_SOURCE_DIR}/textures/brick.png
  ${PROJECT_BINARY_DIR}/brick.png
  COPYONLY
)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY 
${PROJECT_BINARY_DIR}
)
