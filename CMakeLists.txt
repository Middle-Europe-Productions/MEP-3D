cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
project(MEP-3D VERSION 1.0)

macro(mep_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

mep_set_option(MEP_USE_STB TRUE BOOL "TRUE to use stb_image for image processing")

add_subdirectory(third-party/)
add_subdirectory(src/)
add_subdirectory(examples/)
