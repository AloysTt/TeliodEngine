#ifndef TELIOD_PHYSICS_PHYSICSSYSTEM_H
#define TELIOD_PHYSICS_PHYSICSSYSTEM_H

#include <ECS/ECS.h>
#include <physics/CollisionManifold.h>
#include <physics/Rigidbody.h>
#include <functional>

namespace teliod::physics
{
	class PhysicsSystem : public ecs::System
	{
	public:
		PhysicsSystem();

		void update(float dt);

        void addConstraint(const OBB & constraint);
        void clearConstraints();

		void addCallback(ecs::Entity ent, const std::function<void(ecs::Entity)>& callback);

	private:
        std::vector<OBB> constraints;

		std::vector<Rigidbody *> colliders1;
		std::vector<Rigidbody *> colliders2;
		std::vector<CollisionManifold> results;

		std::unordered_map<ecs::Entity, std::function<void(ecs::Entity)>> callbacks;

        float linearProjectionPercent;
        float penetrationSlack;
        int impulseIteration;
	};
}

#endif // TELIOD_PHYSICS_PHYSICSSYSTEM_H
