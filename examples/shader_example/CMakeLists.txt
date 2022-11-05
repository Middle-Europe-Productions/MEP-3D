project(shader_example)

source_group("Shader Files" FILES ${SHADERS})

add_executable(${PROJECT_NAME} main.cpp ${SHADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)
