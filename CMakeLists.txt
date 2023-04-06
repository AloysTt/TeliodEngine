cmake_minimum_required(VERSION 3.23)
project(engine)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

add_executable(engine main.cpp)
message(STATUS <$)
target_link_libraries(engine PRIVATE ECS)
