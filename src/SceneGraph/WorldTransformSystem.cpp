#include <WorldTransformSystem.h>
#include <SceneGraph.h>
#include "Transform.h"

namespace teliod::sg::systems
{
	void WorldTransformSystem::update()
	{
		SceneGraph & sg = SceneGraph::getInstance();
		ecs::World & world = ecs::World::getInstance();

		Node * root = sg.getRoot();
		ecs::Entity rootEntity = root->getEntity();
		glm::mat4x4 startMat{1.0f};
		recurseUpdateWorldTF(startMat, root);
	}

	void WorldTransformSystem::recurseUpdateWorldTF(glm::mat4x4 mat, Node * n)
	{
		ecs::World & world = ecs::World::getInstance();

		Transform & transform = world.getComponent<sg::Transform>(n->getEntity());

		const glm::mat4x4 & local = transform.localTransform;
		mat = mat * local;
		transform.worldTransform = mat;

		for (Node * child : n->getChildren())
		{
			recurseUpdateWorldTF(mat, child);
		}
	}
}
