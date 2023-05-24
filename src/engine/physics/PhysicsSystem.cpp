#include <physics/PhysicsSystem.h>
#include <SceneGraph/Transform.h>

namespace teliod::physics
{
	PhysicsSystem::PhysicsSystem()
	: colliders1()
	, colliders2()
	, results()
    , linearProjectionPercent(0.45f)
    , penetrationSlack(0.05f)
    , impulseIteration(2)
	{
		colliders1.reserve(100);
		colliders2.reserve(100);
		results.reserve(100);
	}

    void PhysicsSystem::update(float dt)
    {
		colliders1.clear();
		colliders2.clear();
		results.clear();

		ecs::World & w = ecs::World::getInstance();
		std::vector<Rigidbody *> bodies;
		std::vector<sg::Transform *> transforms;
		std::vector<ecs::Entity> entities;
		bodies.reserve(getEntities().size());
		for (ecs::Entity e : getEntities())
        {
			bodies.push_back(&w.getComponent<Rigidbody>(e));
			transforms.push_back(&w.getComponent<sg::Transform>(e));
			entities.push_back(e);
        }

		{
			// Find colliding objects
			// First, build a list of colliding objects
			CollisionManifold result;
			for (int i = 0, size = bodies.size(); i < size; ++i)
			{
				for (int j = i; j < size; ++j)
				{
					if (i == j)
						continue;
					result.reset();
					if (bodies[i]->hasVolume() && bodies[j]->hasVolume())
					{
						Rigidbody * m1 = bodies[i];
						Rigidbody * m2 = bodies[j];
						result = findCollisionFeatures(*m1, *m2);
					}
					if (result.colliding)
					{
						colliders1.push_back(bodies[i]);
						colliders2.push_back(bodies[j]);
						if (callbacks.contains(entities[i]))
							callbacks[entities[i]](entities[j]);
						if (callbacks.contains(entities[j]))
							callbacks[entities[j]](entities[i]);
						results.push_back(result);
					}
				}
			}
		}

		// Calculate forces acting on the object
		for (int i=0; i<bodies.size(); ++i)
			bodies[i]->applyForces();

		// Apply impulses to resolve collisions
		for (int k = 0; k < impulseIteration; ++k) { // Apply impulses
			for (int i = 0, size = results.size(); i < size; ++i) {
				for (int j = 0, jSize = results[i].contacts.size(); j < jSize; ++j) {
					if (colliders1[i]->hasVolume() && colliders2[i]->hasVolume()) {
						Rigidbody * m1 = colliders1[i];
						Rigidbody * m2 = colliders2[i];
						applyImpulse(*m1, *m2, results[i], j);
					}
				}
			}
		}

		// Integrate velocity and impulse of objects
		for (int i = 0, size = bodies.size(); i < size; ++i) {
			bodies[i]->update(dt);
		}

		// Correct position to avoid sinking
//		if (doLinearProjection) {
		if (true) {
			for (int i = 0, size = results.size(); i < size; ++i) {
				if (!colliders1[i]->hasVolume() && !colliders2[i]->hasVolume()) {
					continue;
				}

				RigidbodyImplVolume * m1 = static_cast<RigidbodyImplVolume*>(colliders1[i]->getImpl());
				RigidbodyImplVolume * m2 = static_cast<RigidbodyImplVolume*>(colliders2[i]->getImpl());
				float totalMass = m1->invMass() + m2->invMass();

				if (totalMass == 0.0f)
					continue;

				float depth = std::fmaxf(results[i].depth - penetrationSlack, 0.0f);
				float scalar = (totalMass == 0.0f) ? 0.0f : depth / totalMass;
				glm::vec3 correction = results[i].normal * scalar * linearProjectionPercent;

				m1->position = m1->position - correction * m1->invMass();
				m2->position = m2->position + correction * m2->invMass();

				m1->syncCollisionVolumes();
				m2->syncCollisionVolumes();
			}
		}

		// Solve constraints
		for (int i = 0, size = bodies.size(); i < size; ++i) {
			bodies[i]->solveConstraints(constraints);
		}


        // Update objects' transforms
        for (int i = 0, size = bodies.size(); i < size; ++i)
        {
            int test = i;
//            if (!bodies[i]->hasVolume())
            auto * pImpl = static_cast<RigidbodyImplVolume *>(bodies[i]->getImpl());
            if (glm::length2(pImpl->velocity) != 0.0f || glm::length2(pImpl->angVel) != 0.0f)
            {
                sg::Transform * pTransform = transforms[i];
//                glm::mat4 invTransform = glm::inverse(pTransform->getWorldTransform());
                glm::mat4 invTransform = glm::inverse(pTransform->getWorldTransform()*glm::inverse(pTransform->getLocalTransform()));
                pTransform->setPosition(invTransform * glm::vec4(pImpl->box.position, 1.0f));
                pTransform->setRotation(glm::quat(invTransform * glm::mat4(pImpl->box.orientation)));
            }
        }
    }

	void PhysicsSystem::addCallback(ecs::Entity ent, const std::function<void(ecs::Entity)>& callback)
	{
		callbacks.insert({ent, callback});
	}
}
