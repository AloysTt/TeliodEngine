add_subdirectory(glm)
add_subdirectory(glad)
add_subdirectory(glfw)

set(ASSIMP_WARNINGS_AS_ERRORS OFF)
add_subdirectory(assimp)
target_compile_definitions(assimp PRIVATE ASSIMP_WARNINGS_AS_ERRORS OFF)

add_subdirectory(yaml-cpp)
