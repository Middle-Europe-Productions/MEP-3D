cmake_minimum_required(VERSION 3.8)

SET(SOURCE
  glfw_window.cpp
  main.cpp
)

SET(HEADERS
  mesh_base.hpp
  two_dim_mesh.hpp
  three_dim_mesh.hpp
  common.hpp
  drawable.hpp
  pyramid.hpp
  shader.hpp
  space_action.hpp
  triangle.hpp
  utils.hpp
  window.hpp
  window_observer.hpp
  observer_list.hpp
  key_event.hpp
  mouse_event.hpp
)

SET(SHADERS
  shaders/shader.frag
  shaders/shader.vert
)

source_group("Shader Files" FILES ${SHADERS})

source_group("Header Files" FILES ${HEADERS})

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS} ${SHADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} glfw libglew_static glm glog)