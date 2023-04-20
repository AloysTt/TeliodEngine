#ifndef TELIOD_SG_TRANSFORM_H
#define TELIOD_SG_TRANSFORM_H

#include "glm/vec3.hpp"
#include "glm/ext/quaternion_float.hpp"

namespace teliod::sg
{
	namespace systems
	{
		class WorldTransformSystem;
	}

	class Transform
	{
		friend class systems::WorldTransformSystem;
	public:
		inline Transform();
		~Transform() = default;

		inline const glm::vec3 & getPosition() const;
		inline const glm::quat & getRotation() const;
		inline const glm::vec3 & getScale() const;

		inline glm::vec3 getDirection() const;

		inline const glm::mat4 & getLocalTransform() const;
		inline const glm::mat4 & getWorldTransform() const;

		inline void translate(float x, float y, float z);
		inline void translate(const glm::vec3 & translation);
		inline void rotate(float rad, const glm::vec3 & axis);
		inline void scale(float x, float y, float z);
		inline void scale(const glm::vec3 & _scale);

	private:
		glm::vec3 mPosition;
		glm::quat mRotation;
		glm::vec3 mScale;

		glm::mat4 localTransform{1.0};
		glm::mat4 worldTransform{1.0};
	};
}

#include <SceneGraph/Transform.inl>

#endif // TELIOD_SG_TRANSFORM_H
