#ifndef TELIOD_SG_TRANSFORM_H
#define TELIOD_SG_TRANSFORM_H

#include "glm/vec3.hpp"
#include "glm/ext/quaternion_float.hpp"

namespace teliod::sg
{
	struct Transform
	{
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;

		glm::mat4 localTransform{1.0};
		glm::mat4 worldTransform{1.0};
	};
}

#endif // TELIOD_SG_TRANSFORM_H
