set(INCLUDE_DIR include/core)
set(core_sources
		${INCLUDE_DIR}/ResourceManager.h ${INCLUDE_DIR}/ResourceManager.inl
		MeshResource.cpp ${INCLUDE_DIR}/MeshResource.h
		GlobalSettings.cpp ${INCLUDE_DIR}/GlobalSettings.h
)

source_group(core FILES ${core_sources})
add_library(core STATIC ${core_sources})
target_link_libraries(core PUBLIC glm assimp)
target_include_directories(core PUBLIC
		include
)
add_custom_command(TARGET core
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/res/ ${CMAKE_BINARY_DIR}/res/
)
