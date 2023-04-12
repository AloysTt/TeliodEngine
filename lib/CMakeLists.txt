add_subdirectory(glm)
add_subdirectory(glad)
add_subdirectory(glfw)

add_subdirectory(assimp)
target_compile_definitions(assimp PRIVATE ASSIMP_WARNINGS_AS_ERRORS OFF)
