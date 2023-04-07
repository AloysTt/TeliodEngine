set(GLAD_sources
		src/glad.c
		include/glad/glad.h
		include/KHR/khrplatform.h
)

source_group(GLAD FILES ${GLAD_sources})
add_library(GLAD STATIC ${GLAD_sources})
target_include_directories(GLAD PUBLIC
		include
)
