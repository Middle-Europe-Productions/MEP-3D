mep_project(imgui_custom_scene)

source_group("Shader Files")

add_mep_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)
