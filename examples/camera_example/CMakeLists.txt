project(camera_example)

mep_set_output_dir(camera_example)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)
