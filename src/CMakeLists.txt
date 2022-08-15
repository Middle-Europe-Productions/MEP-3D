set(INC_ROOT ${PROJECT_SOURCE_DIR}/include/MEP-3D)
set(SRC_ROOT ${PROJECT_SOURCE_DIR}/src)
set(MEP_LIBRARY_NAME mep-3d)

set(SOURCE
  ${SRC_ROOT}/camera.cpp
  ${SRC_ROOT}/common.cpp
  ${SRC_ROOT}/integration/time_delta_glfw.cpp
  ${SRC_ROOT}/integration/window_glfw.cpp
  ${SRC_ROOT}/mesh_base.cpp
  ${SRC_ROOT}/model_controller.cpp
  ${SRC_ROOT}/figures.cpp
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
  ${SRC_ROOT}/directional_light.cpp
  ${SRC_ROOT}/point_light.cpp
  ${SRC_ROOT}/spot_light.cpp
  ${SRC_ROOT}/asset.cpp
  ${SRC_ROOT}/material.cpp
  ${SRC_ROOT}/thread_pool.cpp
  ${SRC_ROOT}/executor.cpp
  ${SRC_ROOT}/resource_base.cpp
  ${SRC_ROOT}/engine.cpp
  ${SRC_ROOT}/engine_pool.cpp
  ${SRC_ROOT}/layer.cpp
  ${SRC_ROOT}/layer_assets.cpp
  ${SRC_ROOT}/template/layer_assets_ui_layer.cpp
)

set(HEADERS
  ${INC_ROOT}/mesh_base.hpp
  ${INC_ROOT}/mesh.hpp
  ${INC_ROOT}/common.hpp
  ${INC_ROOT}/drawable.hpp
  ${INC_ROOT}/figures.hpp
  ${INC_ROOT}/shader.hpp
  ${INC_ROOT}/model_controller.hpp
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
  ${INC_ROOT}/directional_light.hpp
  ${INC_ROOT}/point_light.hpp
  ${INC_ROOT}/spot_light.hpp
  ${INC_ROOT}/light_controller.hpp
  ${INC_ROOT}/light_controller.inl
  ${INC_ROOT}/asset.hpp
  ${INC_ROOT}/material.hpp
  ${INC_ROOT}/uniform_container.hpp
  ${INC_ROOT}/observable_map.hpp
  ${INC_ROOT}/model.hpp
  ${INC_ROOT}/non_copyable.hpp
  ${INC_ROOT}/resource_base.hpp
  ${INC_ROOT}/thread_pool.hpp
  ${INC_ROOT}/executor.hpp
  ${INC_ROOT}/layer.hpp
  ${INC_ROOT}/engine.hpp
  ${INC_ROOT}/engine_pool.hpp
  ${INC_ROOT}/user_interface.hpp
  ${INC_ROOT}/layer_assets.hpp
  ${INC_ROOT}/template/engine_data_ui_layer.hpp
  ${INC_ROOT}/template/layer_assets_ui_layer.hpp
)

set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}/include
)

set(LINK_TARGETS
    glfw 
    libglew_static 
    glm 
    glog
)

if (MEP_USE_IMGUI)
else()

endif(MEP_USE_IMGUI)

if (MEP_USE_STB)
  list(APPEND SOURCE 
    ${SRC_ROOT}/integration/image_stb.cpp
  )
  list(APPEND INCLUDE_DIRECTORIES 
    ${CMAKE_SOURCE_DIR}/third-party/stb
  )
else()
  list(APPEND SOURCE 
    ${SRC_ROOT}/integration/image_default.cpp
  )
endif(MEP_USE_STB)

if (MEP_USE_ASSIMP)
  list(APPEND SOURCE
    ${SRC_ROOT}/integration/model_assimp.cpp
  )
  list(APPEND LINK_TARGETS
    assimp
  )
else()
  list(APPEND SOURCE
    ${SRC_ROOT}/integration/model_default.cpp
  )
endif(MEP_USE_ASSIMP)

if (MEP_USE_IMGUI)
  add_definitions(-DMEP_ENGINE_USE_IMGUI)
  list(APPEND SOURCE
    ${SRC_ROOT}/integration/user_interface_imgui.cpp
    ${SRC_ROOT}/integration/engine_data_ui_layer_imgui.cpp
    ${SRC_ROOT}/integration/layer_assets_ui_layer_imgui.cpp
  )
  list(APPEND LINK_TARGETS
    imgui
  )
else()
  list(APPEND SOURCE
    ${SRC_ROOT}/integration/user_interface_default.cpp
    ${SRC_ROOT}/integration/engine_data_ui_layer_default.cpp
    ${SRC_ROOT}/integration/layer_assets_ui_layer_default.cpp
  )
endif (MEP_USE_IMGUI)

source_group("Header Files" FILES ${HEADERS})

add_library(${MEP_LIBRARY_NAME} STATIC ${HEADERS} ${SOURCE})

set_target_properties(${MEP_LIBRARY_NAME} PROPERTIES FOLDER "MEP-3D")

target_include_directories(${MEP_LIBRARY_NAME} PUBLIC ${INCLUDE_DIRECTORIES})

target_link_libraries(${MEP_LIBRARY_NAME} PUBLIC ${LINK_TARGETS})
