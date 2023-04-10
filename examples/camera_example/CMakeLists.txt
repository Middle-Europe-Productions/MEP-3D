mep_project(camera_example)

add_mep_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC mep-3d)
