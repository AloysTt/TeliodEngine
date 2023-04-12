set(INCLUDE_DIR include/ECS)
set(ECS_sources
	ECS.cpp ${INCLUDE_DIR}/ECS.h ${INCLUDE_DIR}/ECS.inl
)

source_group(ECS FILES ${ECS_sources})
add_library(ECS STATIC ${ECS_sources})
target_include_directories(ECS PUBLIC
		include
)
