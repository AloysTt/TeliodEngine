cmake_minimum_required(VERSION 3.23)
project(engine)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
add_subdirectory(src)

add_executable(engine main.cpp)
target_link_libraries(engine PRIVATE
		# External
		GLAD
		glfw
		# Internal
		ECS
		SceneGraph
)

# Tests
add_executable(test_ECS test_ECS.cpp)
target_link_libraries(test_ECS PRIVATE ECS)

add_executable(test_SceneGraph test_SceneGraph.cpp)
target_link_libraries(test_SceneGraph PRIVATE ECS SceneGraph)

add_executable(test_Window test_Window.cpp)
target_link_libraries(test_Window PRIVATE ECS SceneGraph GLAD glfw)

add_executable(test_Assimp test_Assimp.cpp)
target_link_libraries(test_Assimp PRIVATE assimp)
