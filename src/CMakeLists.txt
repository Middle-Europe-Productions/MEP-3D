cmake_minimum_required(VERSION 3.8)

SET(SOURCE
  main.cpp
)

SET(HEADERS
  mesh_base.hpp
  two_dim_mesh.hpp
  three_dim_mesh.hpp
  common.hpp
  drawable.hpp
  pyramid.hpp
  shader_wrap.hpp
  space_action.hpp
  triangle.hpp
  utils.hpp
)

source_group("Header Files" FILES ${HEADERS})

add_executable(OpenGL-Basis ${SOURCE} ${HEADERS})

target_link_libraries(OpenGL-Basis glfw libglew_static glm glog)