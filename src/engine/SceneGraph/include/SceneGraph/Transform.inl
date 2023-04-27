#include <SceneGraph/Transform.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace teliod::sg
{
	inline Transform::Transform()
	: mPosition(0.0, 0.0, 0.0)
	, mRotation(1.0f, 0.0f, 0.0f, 0.0f)
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

	inline glm::vec3 Transform::getRotationEuler() const
	{
		return eulerAngles(mRotation);
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

//		localTransform[3] = localTransform[0] * x
//							+ localTransform[1] * y
//							+ localTransform[2] * z
//							+ localTransform[3];

//		mPosition.x = localTransform[3][0];
//		mPosition.y = localTransform[3][1];
//		mPosition.z = localTransform[3][2];
		mPosition.x += x;
		mPosition.y += y;
		mPosition.z += z;

		updateLocalTransform();
	}

	inline void Transform::translate(const glm::vec3 & translation)
	{
		translate(translation[0], translation[1], translation[2]);
	}

	inline void Transform::rotate(float rad, const glm::vec3 & axis)
	{
//		localTransform = glm::rotate(localTransform, rad, axis);
//	 	glm::vec3 _scale, _translation, _skew;
//	 	glm::vec4 _perspective;
//		glm::decompose(localTransform, _scale, mRotation, _translation, _skew, _perspective);

		mRotation = glm::angleAxis(rad, axis) * mRotation;

		updateLocalTransform();
	}

	inline void Transform::scale(float x, float y, float z)
	{
//		localTransform[0] *= x;
//		localTransform[1] *= y;
//		localTransform[2] *= z;

		mScale.x *= x;
		mScale.y *= y;
		mScale.z *= z;

		updateLocalTransform();
	}

	inline void Transform::scale(const glm::vec3 & _scale)
	{
		scale(_scale[0], _scale[1], _scale[2]);
	}

	inline void Transform::setRotation(float x, float y, float z)
	{
		mRotation = glm::quat(glm::eulerAngleXYZ(x, y, z));
		updateLocalTransform();
	}

	inline void Transform::setRotation(const glm::quat & quat)
	{
		mRotation = quat;
		updateLocalTransform();
	}

	glm::vec3 Transform::getDirection() const
	{
		return glm::rotate(getRotation(), glm::vec3(0.0, 0.0, 1.0));
	}

	inline void Transform::updateLocalTransform()
	{
		localTransform = glm::translate(glm::mat4{1.0f}, mPosition) * toMat4(mRotation) * glm::scale(glm::mat4{1.0f}, mScale);
	}
}
