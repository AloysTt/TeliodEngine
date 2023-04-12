#include "SceneGraph/SceneGraph.h"

#include "SceneGraph/Transform.h"
#include "SceneGraph/WorldTransformSystem.h"

namespace teliod::sg
{
	void registerComponents()
	{
		ecs::World & world = ecs::World::getInstance();
		world.registerComponent<Transform>();
		world.registerComponent<SGNode>();
	}

	void registerSystems()
	{
		ecs::World & world = ecs::World::getInstance();

		world.registerSystem<systems::WorldTransformSystem>();
		ecs::Signature s;
		s.set(world.getComponentType<Transform>());
		s.set(world.getComponentType<SGNode>());
		world.setSystemSignature<systems::WorldTransformSystem>(s);
	}
	
	SceneGraph::SceneGraph()
	: mRoot(nullptr)
	{
		mRoot = new Node();
	}

	SceneGraph::~SceneGraph()
	{
		delete mRoot;
	}

	SceneGraph & SceneGraph::getInstance()
	{
		static SceneGraph sg;
		return sg;
	}

	Node *SceneGraph::getRoot()
	{
		return mRoot;
	}

	const std::vector<Node *> & Node::getChildren() const
	{
		return mChildren;
	}

	Node * Node::createChild()
	{
		Node * newNode = new Node();
		mChildren.push_back(newNode);
		return newNode;
	}

	bool Node::destroyChild(ecs::Entity entity)
	{
		int index = getChildIndexFromEntity(entity);
		if (index == -1)
		{
			assert(false && "Destroying non-existing child.");
			return false;
		}

		Node * toDelete = mChildren[index];
		mChildren.erase(mChildren.begin() + index);

		delete toDelete;
		return true;
	}

	int Node::getChildIndexFromEntity(ecs::Entity entity) const
	{
		auto found = std::find_if(
				mChildren.begin(),
				mChildren.end(),
				[entity](const Node * n){
					return n->mEntity == entity;
				});
		if (found != mChildren.end())
			return mChildren.end() - found;
		return -1;
	}

	ecs::Entity Node::getEntity() const
	{
		return mEntity;
	}

	Node::Node()
	: mEntity()
	{
		ecs::World & world = ecs::World::getInstance();
		mEntity = world.createEntity();
		world.addComponent(mEntity, Transform{});
		world.addComponent(mEntity, SGNode{this});
	}

	Node::~Node()
	{
		ecs::World::getInstance().destroyEntity(mEntity);
		for (Node * child : mChildren)
			delete child;
	}
}
