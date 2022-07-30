cmake_minimum_required(VERSION 3.8)

add_subdirectory (glfw/)
add_subdirectory (glew-cmake/)
add_subdirectory (glm/)
add_subdirectory (glog/)

if (MEP_USE_ASSIMP)
    add_subdirectory (assimp/)
endif (MEP_USE_ASSIMP)
