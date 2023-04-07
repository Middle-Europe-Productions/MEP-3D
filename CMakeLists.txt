cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
project(MEP-3D VERSION 1.0)

macro(mep_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

message(STATUS "[MEP] Architecture: ${CMAKE_SYSTEM_PROCESSOR}")

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
    mep_set_option(MEP_USE_ASSIMP FALSE BOOL "TRUE to use Open Asset Import Library for model processing")
    message(WARNING  "[MEP] Disabling assimp library on arm")
else()
    mep_set_option(MEP_USE_ASSIMP TRUE BOOL "TRUE to use Open Asset Import Library for model processing")
endif()

mep_set_option(MEP_USE_STB TRUE BOOL "TRUE to use stb_image for image processing")
mep_set_option(MEP_STANDALONE_APP FALSE BOOL "TRUE when building a standalone application")
mep_set_option(MEP_USE_IMGUI TRUE BOOL "TRUE to use ImGui library for debug output")
mep_set_option(MEP_BUILD_EXAMPLES TRUE BOOL "TRUE to include MEP examples")


add_subdirectory(third-party/)
add_subdirectory(src/)
add_subdirectory(version/)
if(MEP_BUILD_EXAMPLES)
    add_subdirectory(examples/)
endif()
