cmake_minimum_required(VERSION 3.17)
project(engine)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)

add_executable(game
	main.cpp
	src/game/include/game/GameApplication.h
	src/game/GameApplication.cpp
)
target_link_libraries(game PRIVATE
		# External
		GLAD
		glfw
		# Internal
		ECS
		SceneGraph
		core
		physics
)
target_include_directories(game PRIVATE
	src/game/include
)