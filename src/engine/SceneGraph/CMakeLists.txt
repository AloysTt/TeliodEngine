set(INCLUDE_DIR include/SceneGraph)
set(SceneGraph_sources
		SceneGraph.cpp ${INCLUDE_DIR}/SceneGraph.h
		Transform.cpp ${INCLUDE_DIR}/Transform.h ${INCLUDE_DIR}/Transform.inl
		WorldTransformSystem.cpp ${INCLUDE_DIR}/WorldTransformSystem.h
)

source_group(SceneGraph FILES ${SceneGraph_sources})
add_library(SceneGraph STATIC ${SceneGraph_sources})
target_link_libraries(SceneGraph PUBLIC ECS glm)
target_include_directories(SceneGraph PUBLIC
		include
)
