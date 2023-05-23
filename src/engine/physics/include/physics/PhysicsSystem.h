#ifndef TELIOD_PHYSICS_PHYSICSSYSTEM_H
#define TELIOD_PHYSICS_PHYSICSSYSTEM_H

#include <ECS/ECS.h>
#include <physics/CollisionManifold.h>
#include <physics/Rigidbody.h>

namespace teliod::physics
{
	class PhysicsSystem : public ecs::System
	{
	public:
		PhysicsSystem();

		void update(float dt);

        void addConstraint(const OBB & constraint);
        void clearConstraints();

	private:
        std::vector<OBB> constraints;

		std::vector<Rigidbody *> colliders1;
		std::vector<Rigidbody *> colliders2;
		std::vector<CollisionManifold> results;

        float linearProjectionPercent;
        float penetrationSlack;
        int impulseIteration;
	};
}

#endif // TELIOD_PHYSICS_PHYSICSSYSTEM_H
