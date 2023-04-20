#include <game/GameApplication.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "SceneGraph/Transform.h"

#include <ECS/ECS.h>
#include <SceneGraph/SceneGraph.h>
#include <render/Components.h>
#include <core/Components.h>
#include <iostream>

namespace ecs = teliod::ecs;
namespace sg = teliod::sg;
namespace render = teliod::render;
namespace core = teliod::core;

void GameApplication::initInternal()
{
	ecs::World & w = ecs::World::getInstance();
	sg::SceneGraph& sg = sg::SceneGraph::getInstance();
	sg::Node * child = sg.getRoot()->createChild();
	torus = child->getEntity();

	core::MeshComponent meshComp(core::MeshResourceManager::getInstance().getResource("torus"));
	w.addComponent<core::MeshComponent>(torus, core::MeshComponent(core::MeshResourceManager::getInstance().getResource("torus")));
	w.addComponent<render::MeshRenderer>(torus, render::MeshRenderer(torus, render::ShaderResourceManager::getInstance().getResource("default")));
}

void GameApplication::destroyInternal()
{

}

void GameApplication::runInternal(float dt)
{
	std::cout << dt << std::endl;
	ecs::World::getInstance().getComponent<sg::Transform>(torus).rotate(glm::radians(1.0f)*dt, {0.0f, 1.0f, 0.0f});
}
