project(simple_example)

SET(SHADERS
  ${CMAKE_SOURCE_DIR}/shaders/shader.frag
  ${CMAKE_SOURCE_DIR}/shaders/shader.vert
)

source_group("Shader Files" FILES ${SHADERS})

add_executable(${PROJECT_NAME} main.cpp ${SHADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/shaders)
