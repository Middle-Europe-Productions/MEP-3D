cmake_minimum_required(VERSION 3.8)

set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/third-party/imgui)

add_subdirectory (glfw/)
add_subdirectory (glew-cmake/)
add_subdirectory (glm/)
add_subdirectory (glog/)

if (MEP_USE_ASSIMP)
    add_subdirectory (assimp/)
endif (MEP_USE_ASSIMP)

if (MEP_USE_IMGUI)
    add_library(imgui STATIC)
    target_sources(imgui
                PRIVATE
                    ${IMGUI_DIR}/imgui_demo.cpp
                    ${IMGUI_DIR}/imgui_draw.cpp
                    ${IMGUI_DIR}/imgui_tables.cpp
                    ${IMGUI_DIR}/imgui_widgets.cpp
                    ${IMGUI_DIR}/imgui.cpp
                PRIVATE
                    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
                    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
                )
    target_include_directories(imgui
                PUBLIC ${IMGUI_DIR}
                PUBLIC ${IMGUI_DIR}/backends)

    target_link_libraries(imgui PUBLIC glfw)
endif (MEP_USE_IMGUI)
