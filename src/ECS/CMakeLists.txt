set(ECS_sources
	ECS.cpp include/ECS.h include/ECS.inl
)

source_group(ECS FILES ${ECS_sources})
add_library(ECS STATIC ${ECS_sources})
target_include_directories(ECS PUBLIC
		include
)
