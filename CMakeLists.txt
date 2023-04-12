cmake_minimum_required(VERSION 3.23)
project(engine)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)

add_executable(engine main.cpp)
target_link_libraries(engine PRIVATE
		# External
		GLAD
		glfw
		# Internal
		ECS
		SceneGraph
		core
)
