set(INC_ROOT ${PROJECT_SOURCE_DIR}/include/MEP-3D)
set(SRC_ROOT ${PROJECT_SOURCE_DIR}/src)
set(MEP_LIBRARY_NAME mep-3d)

set(SOURCE
  ${SRC_ROOT}/perspective_camera.cpp
  ${SRC_ROOT}/arcball_camera.cpp
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
  ${SRC_ROOT}/utils_engine.cpp
  ${SRC_ROOT}/utils.cpp
  ${SRC_ROOT}/drawable.cpp
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
  ${SRC_ROOT}/layer.cpp
  ${SRC_ROOT}/scene.cpp
  ${SRC_ROOT}/common_names.cpp
  ${SRC_ROOT}/config_create_impl.cpp
  ${SRC_ROOT}/parser.cpp
  ${SRC_ROOT}/key_event.cpp
  ${SRC_ROOT}/camera_base.cpp
  ${SRC_ROOT}/features.cpp
  ${SRC_ROOT}/async_progress.cpp
  ${SRC_ROOT}/file_reader_with_progress.cpp
  ${SRC_ROOT}/texture_3d.cpp
  ${SRC_ROOT}/volume.cpp
  ${SRC_ROOT}/texture_utils.cpp
  ${SRC_ROOT}/texture_view.cpp
# template/
  ${SRC_ROOT}/template/scene_ui_layer.cpp
  ${SRC_ROOT}/template/shaders_preset.cpp
  ${SRC_ROOT}/template/ui_element.cpp
# ilc/
  ${SRC_ROOT}/ilc/ilc_client.cpp
  ${SRC_ROOT}/ilc/ilc_daemon_default.cpp
  ${SRC_ROOT}/ilc/ilc_connector.cpp
  # platform
  ${SRC_ROOT}/platform/platform_delegate.cpp
  ${SRC_ROOT}/platform/platform_utils_generic.cpp
)

if (WIN32)
list(APPEND SOURCE 
  # platform
  ${SRC_ROOT}/platform/platform_delegate_windows.cpp
)
elseif(APPLE)
list(APPEND SOURCE 
  # platform
  ${SRC_ROOT}/platform/platform_delegate_macos.mm
)
else()
list(APPEND SOURCE 
  # platform
  ${SRC_ROOT}/platform/platform_delegate_generic.cpp
)
endif()

set(HEADERS
  ${INC_ROOT}/mep-3d.hpp
  ${INC_ROOT}/mesh_base.hpp
  ${INC_ROOT}/mesh.hpp
  ${INC_ROOT}/common.hpp
  ${INC_ROOT}/drawable.hpp
  ${INC_ROOT}/figures.hpp
  ${INC_ROOT}/shader.hpp
  ${INC_ROOT}/model_controller.hpp
  ${INC_ROOT}/utils_engine.hpp
  ${INC_ROOT}/utils.hpp
  ${INC_ROOT}/window.hpp
  ${INC_ROOT}/window_observer.hpp
  ${INC_ROOT}/observer_list.hpp
  ${INC_ROOT}/key_event.hpp
  ${INC_ROOT}/mouse_event.hpp
  ${INC_ROOT}/perspective_camera.hpp
  ${INC_ROOT}/arcball_camera.hpp
  ${INC_ROOT}/camera_base.hpp
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
  ${INC_ROOT}/user_interface.hpp
  ${INC_ROOT}/scene.hpp
  ${INC_ROOT}/common_names.hpp
  ${INC_ROOT}/parser.hpp
  ${INC_ROOT}/features.hpp
  ${INC_ROOT}/async_progress.hpp
  ${INC_ROOT}/file_reader_with_progress.hpp
  ${INC_ROOT}/texture_3d.hpp
  ${INC_ROOT}/volume.hpp
  ${INC_ROOT}/texture_utils.hpp
  ${INC_ROOT}/texture_view.hpp
  ${INC_ROOT}/parser_handler.hpp
# template/
  ${INC_ROOT}/template/engine_data_ui_layer.hpp
  ${INC_ROOT}/template/scene_ui_layer.hpp
  ${INC_ROOT}/template/util_common_draw.hpp
  ${INC_ROOT}/template/shaders_preset.hpp
  ${INC_ROOT}/template/ui_element.hpp
  ${INC_ROOT}/template/scene_ui_layer_handler_context.hpp
# ilc/
  ${INC_ROOT}/ilc/ilc_client.hpp
  ${INC_ROOT}/ilc/ilc_daemon.hpp
  ${INC_ROOT}/ilc/ilc_connector.hpp
# platform
  ${INC_ROOT}/platform/platform_delegate.hpp
  ${INC_ROOT}/platform/platform_utils.hpp
)

set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}/include
)

set(LINK_TARGETS
    glfw 
    libglew_static 
    glm 
    glog::glog
    nlohmann_json::nlohmann_json
)

add_definitions(-DMEP_USES_GLFW -DMEP_USES_FLEW -DMEP_USES_GLM -DMEP_USES_GLOG -DMEP_USES_NLOHMANN_JSON)

if (MEP_USE_STB)
  message(STATUS "MEP_USE_STB=true, including stb library for image loading")
  list(APPEND SOURCE 
    ${SRC_ROOT}/integration/image_stb.cpp
  )
  list(APPEND INCLUDE_DIRECTORIES 
    ${CMAKE_SOURCE_DIR}/third-party/stb
  )
  add_definitions(-DMEP_USES_STB)
else()
message(STATUS "Library for image processing is not included linking default implementation")
  list(APPEND SOURCE 
    ${SRC_ROOT}/integration/image_default.cpp
  )
endif(MEP_USE_STB)

if (MEP_USE_ASSIMP)
  message(STATUS "MEP_USE_ASSIMP=true, including assimp library for model loading")
  list(APPEND SOURCE
    ${SRC_ROOT}/integration/model_assimp.cpp
  )
  list(APPEND LINK_TARGETS
    assimp
  )
  add_definitions(-DMEP_USES_ASSIMP)
else()
  message(STATUS "Library for model processing is not included linking default implementation")
  list(APPEND SOURCE
    ${SRC_ROOT}/integration/model_default.cpp
  )
endif(MEP_USE_ASSIMP)

if (NOT MEP_STANDALONE_APP)
  message(STATUS "MEP_STANDALONE_APP=false, including imgui scene")
  add_definitions(-DMEP_ENGINE_USE_IMGUI)
  set(MEP_USE_IMGUI TRUE)
  list(APPEND SOURCE
    ${SRC_ROOT}/integration/imgui/user_interface_imgui.cpp
    ${SRC_ROOT}/integration/imgui/engine_data_ui_layer_imgui.cpp
    ${SRC_ROOT}/integration/imgui/scene_ui_layer_imgui.cpp
    ${SRC_ROOT}/integration/imgui/util_common_draw_imgui.cpp
    ${SRC_ROOT}/integration/imgui/imgui_addons.cpp
    ${SRC_ROOT}/integration/imgui/imgui_addons.hpp
    ${SRC_ROOT}/integration/imgui/parser_imgui.cpp
    ${SRC_ROOT}/integration/imgui/parser_imgui.hpp
    ${SRC_ROOT}/integration/imgui/scene_ui_layer_imgui.cpp
    ${SRC_ROOT}/integration/imgui/enum_widget.hpp
    # handlers
    ${SRC_ROOT}/integration/imgui/handlers/ui_handlers.cpp
    ${SRC_ROOT}/integration/imgui/handlers/resource_wrap.hpp
    ${SRC_ROOT}/integration/imgui/handlers/resource_wrap.cpp
    ${SRC_ROOT}/integration/imgui/handlers/volume_wrap.hpp
    ${SRC_ROOT}/integration/imgui/handlers/volume_wrap.cpp
    ${SRC_ROOT}/integration/imgui/handlers/shader_editor_context.hpp
    ${SRC_ROOT}/integration/imgui/handlers/shader_editor_context.cpp
  )

else()
  message(STATUS "MEP_STANDALONE_APP=true, removing scene implementation")
  list(APPEND SOURCE
    ${SRC_ROOT}/integration/default/user_interface_default.cpp
    ${SRC_ROOT}/integration/default/engine_data_ui_layer_default.cpp
    ${SRC_ROOT}/integration/default/scene_ui_layer_default.cpp
    ${SRC_ROOT}/integration/default/util_common_draw_default.cpp
  )
endif ()

if (MEP_USE_IMGUI)
  message(STATUS "MEP_USE_IMGUI=true, including imgui library")
  list(APPEND LINK_TARGETS
    imgui
    imgui-widget
  )
  add_definitions(-DMEP_USES_IMGUI)
endif () 

source_group("Header Files" FILES ${HEADERS})

add_library(${MEP_LIBRARY_NAME} STATIC ${HEADERS} ${SOURCE})

set_target_properties(${MEP_LIBRARY_NAME} PROPERTIES FOLDER "MEP-3D")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
message(STATUS "[MEP] Detected release target")
target_compile_options(${MEP_LIBRARY_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /GL /O2 /DNDEBUG>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-O3 -Wall -Wextra -Wpedantic -Werror -Wno-deprecated-declarations>
)
else()
message(STATUS "[MEP] Detected debug target")
target_compile_options(${MEP_LIBRARY_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /p:Configuration=Debug>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror -Wno-deprecated-declarations>
)
endif()

target_include_directories(${MEP_LIBRARY_NAME} PUBLIC ${INCLUDE_DIRECTORIES})

target_link_libraries(${MEP_LIBRARY_NAME} PUBLIC ${LINK_TARGETS})
