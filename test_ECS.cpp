#include "ECS/ECS.h"
#include <string>
#include <iostream>

struct IntComponent
{
	int val;
};
struct StringComponent
{
	std::string val;
};
struct ArrayComponent
{
	int val[4];
};

class TestSystem : public teliod::ecs::System
{
public:
	void update()
	{
		int i=1;
		for (const teliod::ecs::Entity & e : getEntities())
		{
			std::cout << "entity " << std::to_string(i++) << std::endl;
		}
	}
};


int main(int argc, char **argv)
{
	teliod::ecs::World & w = teliod::ecs::World::getInstance();
	w.registerComponent<IntComponent>();
	w.registerComponent<StringComponent>();
	w.registerComponent<ArrayComponent>();

	TestSystem * system = w.registerSystem<TestSystem>();


	teliod::ecs::Signature s;
	s.set(w.getComponentType<IntComponent>());
	s.set(w.getComponentType<StringComponent>());
	s.set(w.getComponentType<ArrayComponent>());
	w.setSystemSignature<TestSystem>(s);

	std::vector<teliod::ecs::Entity> entities; // "scene graph"

	// create 6 entities with the right components for the system
	for (auto i : {1, 2, 3, 4, 5, 6})
	{
		teliod::ecs::Entity entity = w.createEntity();
		w.addComponent(entity, IntComponent{i});
		w.addComponent(entity, StringComponent{std::string("aaaa", i)});
		w.addComponent(entity, ArrayComponent{0, 1, 2, 3});
		entities.push_back(entity);
	}

	// create one that does not
	teliod::ecs::Entity entity2 = w.createEntity();
	w.addComponent(entity2, IntComponent{123});
	w.addComponent(entity2, StringComponent{"wrong one"});

	// we should get 6 entities in the output
	system->update();

	std::cout << std::endl;

	// remove a component from one of the entities
	w.removeComponent<IntComponent>(entities[0]);

	// we should now get 5 entities in the output
	system->update();
	return 0;
}
