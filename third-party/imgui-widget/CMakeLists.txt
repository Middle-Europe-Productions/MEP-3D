cmake_minimum_required(VERSION 3.8)

set(WIDGET_DIR ${PROJECT_SOURCE_DIR}/third-party/imgui-widget)

add_library(imgui-widget STATIC)

target_sources(imgui-widget PRIVATE
    ${WIDGET_DIR}/text-editor/TextEditor.cpp
)

target_include_directories(imgui-widget PUBLIC
    ${WIDGET_DIR}/
)

target_link_libraries(imgui-widget PUBLIC imgui)
