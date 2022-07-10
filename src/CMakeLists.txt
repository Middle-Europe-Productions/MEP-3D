set(INC_ROOT ${PROJECT_SOURCE_DIR}/include/MEP-3D)
set(SRC_ROOT ${PROJECT_SOURCE_DIR}/src)
set(MEP_LIBRARY_NAME mep-3d)

SET(SOURCE
  ${SRC_ROOT}/camera.cpp
  ${SRC_ROOT}/common.cpp
  ${SRC_ROOT}/glfw_time_delta.cpp
  ${SRC_ROOT}/glfw_window.cpp
  ${SRC_ROOT}/mesh_base.cpp
  ${SRC_ROOT}/object_action.cpp
  ${SRC_ROOT}/pyramid.cpp
  ${SRC_ROOT}/render_target.cpp
  ${SRC_ROOT}/shader.cpp
  ${SRC_ROOT}/three_dim_mesh.cpp
  ${SRC_ROOT}/three_dim_view.cpp
  ${SRC_ROOT}/triangle.cpp
  ${SRC_ROOT}/two_dim_mesh.cpp
  ${SRC_ROOT}/updatable.cpp
  ${SRC_ROOT}/utils.cpp
)

SET(HEADERS
  ${INC_ROOT}/mesh_base.hpp
  ${INC_ROOT}/two_dim_mesh.hpp
  ${INC_ROOT}/three_dim_mesh.hpp
  ${INC_ROOT}/common.hpp
  ${INC_ROOT}/drawable.hpp
  ${INC_ROOT}/pyramid.hpp
  ${INC_ROOT}/shader.hpp
  ${INC_ROOT}/object_action.hpp
  ${INC_ROOT}/triangle.hpp
  ${INC_ROOT}/utils.hpp
  ${INC_ROOT}/window.hpp
  ${INC_ROOT}/window_observer.hpp
  ${INC_ROOT}/observer_list.hpp
  ${INC_ROOT}/key_event.hpp
  ${INC_ROOT}/mouse_event.hpp
  ${INC_ROOT}/camera.hpp
  ${INC_ROOT}/render_target.hpp
  ${INC_ROOT}/three_dim_view.hpp
  ${INC_ROOT}/time_delta.hpp
  ${INC_ROOT}/updatable.hpp
  ${INC_ROOT}/view_base.hpp
)

source_group("Header Files" FILES ${HEADERS})

add_library(${MEP_LIBRARY_NAME} STATIC ${HEADERS} ${SOURCE})

set_target_properties(${MEP_LIBRARY_NAME} PROPERTIES FOLDER "MEP-3D")

target_include_directories(${MEP_LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${MEP_LIBRARY_NAME} PUBLIC glfw libglew_static glm glog)
