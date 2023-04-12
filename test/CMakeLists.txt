add_executable(test_ECS test_ECS.cpp)
target_link_libraries(test_ECS PRIVATE ECS)

add_executable(test_SceneGraph test_SceneGraph.cpp)
target_link_libraries(test_SceneGraph PRIVATE ECS SceneGraph)

add_executable(test_Window test_Window.cpp)
target_link_libraries(test_Window PRIVATE ECS SceneGraph GLAD glfw)

add_executable(test_Assimp test_Assimp.cpp)
target_link_libraries(test_Assimp PRIVATE assimp)

add_executable(test_ResManager test_ResManager.cpp)
target_link_libraries(test_ResManager PRIVATE core)
