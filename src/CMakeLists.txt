set(INC_ROOT ${PROJECT_SOURCE_DIR}/include/MEP-3D)
set(SRC_ROOT ${PROJECT_SOURCE_DIR}/src)
set(MEP_LIBRARY_NAME mep-3d)

set(SOURCE
  ${SRC_ROOT}/camera.cpp
  ${SRC_ROOT}/common.cpp
  ${SRC_ROOT}/glfw_time_delta.cpp
  ${SRC_ROOT}/glfw_window.cpp
  ${SRC_ROOT}/mesh_base.cpp
  ${SRC_ROOT}/object_action.cpp
  ${SRC_ROOT}/pyramid.cpp
  ${SRC_ROOT}/render_target.cpp
  ${SRC_ROOT}/shader.cpp
  ${SRC_ROOT}/mesh.cpp
  ${SRC_ROOT}/perspective_view.cpp
  ${SRC_ROOT}/updatable.cpp
  ${SRC_ROOT}/utils.cpp
  ${SRC_ROOT}/texture.cpp
  ${SRC_ROOT}/identity.cpp
  ${SRC_ROOT}/observer.cpp
  ${SRC_ROOT}/asset_controller.cpp
  ${SRC_ROOT}/light.cpp
  ${SRC_ROOT}/asset.cpp
  ${SRC_ROOT}/material.cpp
)

set(HEADERS
  ${INC_ROOT}/mesh_base.hpp
  ${INC_ROOT}/mesh.hpp
  ${INC_ROOT}/common.hpp
  ${INC_ROOT}/drawable.hpp
  ${INC_ROOT}/pyramid.hpp
  ${INC_ROOT}/shader.hpp
  ${INC_ROOT}/object_action.hpp
  ${INC_ROOT}/utils.hpp
  ${INC_ROOT}/window.hpp
  ${INC_ROOT}/window_observer.hpp
  ${INC_ROOT}/observer_list.hpp
  ${INC_ROOT}/key_event.hpp
  ${INC_ROOT}/mouse_event.hpp
  ${INC_ROOT}/camera.hpp
  ${INC_ROOT}/render_target.hpp
  ${INC_ROOT}/perspective_view.hpp
  ${INC_ROOT}/time_delta.hpp
  ${INC_ROOT}/updatable.hpp
  ${INC_ROOT}/view_base.hpp
  ${INC_ROOT}/image.hpp
  ${INC_ROOT}/vector.hpp
  ${INC_ROOT}/vector.inl
  ${INC_ROOT}/texture.hpp
  ${INC_ROOT}/color.hpp
  ${INC_ROOT}/identity.hpp
  ${INC_ROOT}/asset_observer.hpp
  ${INC_ROOT}/asset_controller.hpp
  ${INC_ROOT}/asset_controller.inl
  ${INC_ROOT}/observer.hpp
  ${INC_ROOT}/light.hpp
  ${INC_ROOT}/asset.hpp
  ${INC_ROOT}/material.hpp
  ${INC_ROOT}/uniform_container.hpp
)

set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}/include
)

if (MEP_USE_STB)
  list(APPEND SOURCE 
    ${SRC_ROOT}/stb_image.cpp
  )
  list(APPEND INCLUDE_DIRECTORIES 
    ${CMAKE_SOURCE_DIR}/third-party/stb
  )
else()
  list(APPEND SOURCE 
    ${SRC_ROOT}/default_image.cpp
  )
endif(MEP_USE_STB)

source_group("Header Files" FILES ${HEADERS})

add_library(${MEP_LIBRARY_NAME} STATIC ${HEADERS} ${SOURCE})

set_target_properties(${MEP_LIBRARY_NAME} PROPERTIES FOLDER "MEP-3D")

target_include_directories(${MEP_LIBRARY_NAME} PUBLIC ${INCLUDE_DIRECTORIES})

target_link_libraries(${MEP_LIBRARY_NAME} PUBLIC glfw libglew_static glm glog)
