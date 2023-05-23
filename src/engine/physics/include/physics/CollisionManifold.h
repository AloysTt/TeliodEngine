#ifndef TELIOD_PHYSICS_COLLISIONMANIFOLD_H
#define TELIOD_PHYSICS_COLLISIONMANIFOLD_H

#include <vector>
#include <glm/vec3.hpp>

namespace teliod::physics
{
	struct CollisionManifold
	{
		bool colliding;
		glm::vec3 normal;
		float depth;
		std::vector<glm::vec3> contacts;

		inline void reset()
		{
			colliding = false;
			normal = glm::vec3(0, 0, 1);
			depth = FLT_MAX;
			if (contacts.size() > 0)
			{
				contacts.clear();
			}
		}
	};
}

#endif // TELIOD_PHYSICS_COLLISIONMANIFOLD_H
