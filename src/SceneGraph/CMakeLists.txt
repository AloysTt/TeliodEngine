set(SceneGraph_sources
		SceneGraph.cpp include/SceneGraph/SceneGraph.h
		include/SceneGraph/Transform.h
		WorldTransformSystem.cpp include/SceneGraph/WorldTransformSystem.h
)

source_group(SceneGraph FILES ${SceneGraph_sources})
add_library(SceneGraph STATIC ${SceneGraph_sources})
target_link_libraries(SceneGraph PUBLIC ECS glm)
target_include_directories(SceneGraph PUBLIC
		include
)
