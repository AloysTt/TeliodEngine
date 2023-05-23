#ifndef TELIOD_PHYSICS_GEOMETRY_H
#define TELIOD_PHYSICS_GEOMETRY_H

#include <physics/CollisionManifold.h>
#include "glm/ext/quaternion_float.hpp"
#include "glm/gtc/quaternion.hpp"

namespace teliod::physics
{
	class Line
	{
	public:
		inline Line()
		{
		}

		inline Line(const glm::vec3 & s, const glm::vec3 & e)
		: start(s)
		, end(e)
		{
		}

	public:
		glm::vec3 start;
		glm::vec3 end;
	};


	class Plane
	{
	public:
		inline Plane()
		: normal(1, 0, 0)
		, distance(0)
		{
		}

		inline Plane(const glm::vec3 & n, float d)
		: normal(n)
		, distance(d)
		{
		}

	public:
		glm::vec3 normal;
		float distance;
	};

	class Sphere
	{
	public:
		inline Sphere()
		: radius(1.0f)
		, position(0.0f, 0.0f, 0.0f)
		{
		}

		inline Sphere(const glm::vec3 & p, float r)
		: position(p)
		, radius(r)
		{
		}

	public:
		glm::vec3 position;
		float radius;
	};

	class OBB
	{
	public:
		inline OBB()
		: position(0.0f, 0.0f, 0.0f)
		, size(1.0f, 1.0f, 1.0f)
		, orientation(1.0f)
		, orientationQuat()
		{
			orientationQuat = glm::quat(orientation);
		}

		inline OBB(const glm::vec3 & p, const glm::vec3 & s)
		: position(p)
		, size(s)
		, orientation(1.0f)
		, orientationQuat()
		{
			orientationQuat = glm::quat(orientation);
		}

		inline OBB(const glm::vec3 & p, const glm::vec3& s, const glm::mat3 & o)
		: position(p)
		, size(s)
		, orientation(o)
		, orientationQuat()
		{
			orientationQuat = glm::quat(orientation);
		}

	public:
		glm::vec3 position;
		glm::vec3 size; // half size
		glm::quat orientationQuat;
		glm::mat3 orientation;
	};

	typedef struct Interval {
		float min;
		float max;
	} Interval;

	Interval getInterval(const OBB& obb, const glm::vec3 & axis);
	glm::vec3 closestPoint(const OBB & obb, const glm::vec3 & point);
	bool sphereSphere(const Sphere & s1, const Sphere & s2);
	std::vector<glm::vec3> getVertices(const OBB& obb);
	std::vector<Line> getEdges(const OBB & obb);
	float penetrationDepth(const OBB& o1, const OBB& o2, const glm::vec3 & axis, bool* outShouldFlip);
	std::vector<Plane> getPlanes(const OBB& obb);
	bool clipToPlane(const Plane& plane, const Line& line, glm::vec3 * outPoint);
	bool pointInOBB(const glm::vec3 & point, const OBB& obb);
	std::vector<glm::vec3> clipEdgesToOBB(const std::vector<Line>& edges, const OBB& obb);

	CollisionManifold findCollisionFeatures(const Sphere& A, const Sphere& B);
	CollisionManifold findCollisionFeatures(const OBB& A, const Sphere& B);
	CollisionManifold findCollisionFeatures(const OBB& A, const OBB& B);

	inline bool AlmostEqualRelativeAndAbs(float A, float B, float maxDiff, float maxRelDiff = FLT_EPSILON);
}

#include <physics/Geometry.inl>

#define CMP(x, y) \
	AlmostEqualRelativeAndAbs(x, y, 0.005f)

#endif // TELIOD_PHYSICS_GEOMETRY_H
