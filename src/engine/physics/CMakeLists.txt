set(INCLUDE_DIR include/physics)
set(physics_sources
		${INCLUDE_DIR}/PhysicsSystem.h PhysicsSystem.cpp
		${INCLUDE_DIR}/Rigidbody.h ${INCLUDE_DIR}/Rigidbody.inl Rigidbody.cpp
		${INCLUDE_DIR}/CollisionManifold.h
		${INCLUDE_DIR}/Geometry.h ${INCLUDE_DIR}/Geometry.inl Geometry.cpp
)

source_group(physics FILES ${physics_sources})
add_library(physics STATIC ${physics_sources})
target_include_directories(physics PUBLIC
		include
)
target_link_libraries(physics PRIVATE
		ECS
		glm
		SceneGraph
)
