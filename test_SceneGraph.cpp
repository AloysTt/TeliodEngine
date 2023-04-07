#include <SceneGraph.h>
#include <WorldTransformSystem.h>
#include <Transform.h>
#include <iostream>
#include <string>

using namespace teliod;

void printWorldPos(const sg::Transform & tf)
{
	std::cout << "(" << std::to_string(tf.worldTransform[3][0]) << " " << std::to_string(tf.worldTransform[3][1]) << " " << std::to_string(tf.worldTransform[3][2]) << ")\n";
}

int main(int argc, char **argv)
{
	// intialization
	sg::registerComponents();
//	sg::registerSystems();

	ecs::World & world = ecs::World::getInstance();
	auto worldTFsystem = world.registerSystem<sg::systems::WorldTransformSystem>();
	sg::Node * root = sg::SceneGraph::getInstance().getRoot();

	sg::Node * node1 = root->createChild();
	sg::Transform & tf1 = world.getComponent<sg::Transform>(node1->getEntity());
	sg::Node * node2 = node1->createChild();
	sg::Transform & tf2 = world.getComponent<sg::Transform>(node2->getEntity());

	worldTFsystem->update();
	printWorldPos(tf1);
	printWorldPos(tf2);
	std::cout << std::endl;

	std::cout << "move node 1 local pos (+1 on X)" << std::endl;
	tf1.localTransform[3][0]+=1;
	worldTFsystem->update();
	printWorldPos(tf1);
	printWorldPos(tf2);
	std::cout << std::endl;

	std::cout << "move node 2 local pos (+1 on X)" << std::endl;
	tf2.localTransform[3][0]+=1;
	worldTFsystem->update();
	printWorldPos(tf1);
	printWorldPos(tf2);
	std::cout << std::endl;

	std::cout << "rotate node 1 around Y axis (45 degrees)" << std::endl;
	tf1.localTransform = glm::rotate(tf1.localTransform, glm::radians(45.0f), glm::vec3{0.0f, 1.0f, 0.0f});
	worldTFsystem->update();
	printWorldPos(tf1);
	printWorldPos(tf2);
	std::cout << std::endl;

	return 0;
}
