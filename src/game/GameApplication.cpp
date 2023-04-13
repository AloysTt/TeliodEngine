#include <game/GameApplication.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <ECS/ECS.h>
#include <SceneGraph/SceneGraph.h>
#include <render/Components.h>
#include <core/Components.h>

namespace ecs = teliod::ecs;
namespace sg = teliod::sg;
namespace render = teliod::render;
namespace core = teliod::core;

void GameApplication::initInternal()
{
	ecs::World & w = ecs::World::getInstance();
	sg::SceneGraph& sg = sg::SceneGraph::getInstance();
	sg::Node * child = sg.getRoot()->createChild();
	ecs::Entity e = child->getEntity();

	core::MeshComponent meshComp(core::MeshResourceManager::getInstance().getResource("torus"));
	w.addComponent<core::MeshComponent>(e, core::MeshComponent(core::MeshResourceManager::getInstance().getResource("torus")));
	w.addComponent<render::MeshRenderer>(e, render::MeshRenderer(e, render::ShaderResourceManager::getInstance().getResource("default")));
}

void GameApplication::destroyInternal()
{

}

void GameApplication::runInternal()
{
}
