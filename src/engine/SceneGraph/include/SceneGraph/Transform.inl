#include <SceneGraph/Transform.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace teliod::sg
{
	inline Transform::Transform()
	: mPosition(0.0, 0.0, 0.0)
	, mRotation(0.0f, 0.0f, 0.0f, 1.0f)
	, mScale(1.0, 1.0, 1.0)
	, localTransform(1.0)
	, worldTransform(1.0)
	{
	}

	inline const glm::vec3 & Transform::getPosition() const
	{
		return mPosition;
	}

	inline const glm::quat & Transform::getRotation() const
	{
		return mRotation;
	}

	inline const glm::vec3 & Transform::getScale() const
	{
		return mScale;
	}

	inline const glm::mat4 &Transform::getLocalTransform() const
	{
		return localTransform;
	}

	inline const glm::mat4 &Transform::getWorldTransform() const
	{
		return worldTransform;
	}

	inline void Transform::translate(float x, float y, float z)
	{

		localTransform[3] = localTransform[0] * x
							+ localTransform[1] * y
							+ localTransform[2] * z
							+ localTransform[3];

		mPosition.x = localTransform[3][0];
		mPosition.y = localTransform[3][1];
		mPosition.z = localTransform[3][2];
	}

	inline void Transform::translate(const glm::vec3 & translation)
	{
		translate(translation[0], translation[1], translation[2]);
	}

	inline void Transform::rotate(float rad, const glm::vec3 & axis)
	{
		localTransform = glm::rotate(localTransform, rad, axis);
	 	glm::vec3 _scale, _translation, _skew;
	 	glm::vec4 _perspective;
		glm::decompose(localTransform, _scale, mRotation, _translation, _skew, _perspective);
	}

	inline void Transform::scale(float x, float y, float z)
	{
		localTransform[0] *= x;
		localTransform[1] *= y;
		localTransform[2] *= z;

		mScale.x *= x;
		mScale.y *= y;
		mScale.z *= z;
	}

	inline void Transform::scale(const glm::vec3 & _scale)
	{
		scale(_scale[0], _scale[1], _scale[2]);
	}

	glm::vec3 Transform::getDirection() const
	{
		return glm::rotate(glm::inverse(getRotation()), glm::vec3(0.0, 0.0, 1.0));
	}
}
