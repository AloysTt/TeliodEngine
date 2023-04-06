cmake_minimum_required(VERSION 3.23)
project(engine)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

add_executable(engine main.cpp)
target_link_libraries(engine PRIVATE ECS)

# Tests
add_executable(test_ECS test_ECS.cpp)
target_link_libraries(test_ECS PRIVATE ECS)
