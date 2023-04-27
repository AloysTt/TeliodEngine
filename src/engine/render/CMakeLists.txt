set(INCLUDE_DIR include/render)
set(render_sources
		MeshRendererSystem.cpp ${INCLUDE_DIR}/MeshRendererSystem.h
		RenderBackend.cpp ${INCLUDE_DIR}/RenderBackend.h
		Components.cpp ${INCLUDE_DIR}/Components.h
		ShaderResource.cpp ${INCLUDE_DIR}/ShaderResource.h
		TextureResource.cpp ${INCLUDE_DIR}/TextureResource.h
		${INCLUDE_DIR}/stb_image.h
)

source_group(render FILES ${render_sources})
add_library(render STATIC ${render_sources})
target_link_libraries(render PUBLIC
		core
		GLAD
		glfw
)
target_link_libraries(render PRIVATE yaml-cpp)
target_include_directories(render PUBLIC
		include
)
add_custom_command(TARGET render
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/res/ ${CMAKE_BINARY_DIR}/res/
)
