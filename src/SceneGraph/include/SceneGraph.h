#ifndef TELIOD_SG_SCENEGRAPH__H
#define TELIOD_SG_SCENEGRAPH__H

#include <ECS.h>

namespace teliod::sg
{
	void registerComponents();
	void registerSystems();

	class Node
	{
		friend class SceneGraph;
	public:
		const std::vector<Node *> & getChildren() const;

		Node * createChild();
		bool destroyChild(ecs::Entity entity);

		ecs::Entity getEntity() const;

	private:
		Node();
		~Node();

		std::vector<Node *> mChildren;
		ecs::Entity mEntity;

		// utility
		int getChildIndexFromEntity(ecs::Entity entity) const;
	};

	class SceneGraph
	{
	public:
		static SceneGraph & getInstance();

		Node * getRoot();

		SceneGraph & operator=(const SceneGraph & other) = delete;
		SceneGraph & operator=(SceneGraph && other) = delete;
		SceneGraph(SceneGraph && other) = delete;
		SceneGraph(const SceneGraph & other) = delete;

	private:
		SceneGraph();
		~SceneGraph();

		Node * mRoot;
	};


	// Node component for ECS
	struct SGNode
	{
		Node * node;
	};
}

#endif //TELIOD_SG_SCENEGRAPH__H
