#include <physics/Geometry.h>
#include "glm/gtx/norm.hpp"

namespace teliod::physics
{
	Interval getInterval(const OBB& obb, const glm::vec3 & axis)
	{
		glm::vec3 vertex[8];

		glm::vec3 C = obb.position;	// OBB Center
		glm::vec3 E = obb.size;		// OBB Extents
		const glm::mat3 & o = obb.orientation;
		glm::vec3 A[] = {			// OBB Axis
				glm::vec3(o[0][0], o[0][1], o[0][2]),
				glm::vec3(o[1][0], o[1][1], o[1][2]),
				glm::vec3(o[2][0], o[2][1], o[2][2]),
		};

		vertex[0] = C + A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
		vertex[1] = C - A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
		vertex[2] = C + A[0] * E[0] - A[1] * E[1] + A[2] * E[2];
		vertex[3] = C + A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
		vertex[4] = C - A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
		vertex[5] = C + A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
		vertex[6] = C - A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
		vertex[7] = C - A[0] * E[0] - A[1] * E[1] + A[2] * E[2];

		Interval result;
		result.min = result.max = glm::dot(axis, vertex[0]);

		for (int i = 1; i < 8; ++i) {
			float projection = glm::dot(axis, vertex[i]);
			result.min = (projection < result.min) ? projection : result.min;
			result.max = (projection > result.max) ? projection : result.max;
		}

		return result;
	}

	glm::vec3 closestPoint(const OBB& obb, const glm::vec3 & point)
	{
		glm::vec3 result = obb.position;
		glm::vec3 dir = point - obb.position;

		for (int i = 0; i < 3; ++i) {
			glm::vec3 axis(obb.orientation[i][0], obb.orientation[i][1], obb.orientation[i][2]);

			float distance = glm::dot(dir, axis);

			if (distance > obb.size[i])
				distance = obb.size[i];
			if (distance < -obb.size[i])
				distance = -obb.size[i];

			result = result + (axis * distance);
		}

		return result;
	}

	bool sphereSphere(const Sphere & s1, const Sphere & s2)
	{
		float radiiSum = s1.radius + s2.radius;
		float sqDistance = glm::length2(s1.position - s2.position);
		return sqDistance < radiiSum * radiiSum;
	}

	std::vector<glm::vec3> getVertices(const OBB& obb)
	{
		std::vector<glm::vec3> v;
		v.resize(8);

		glm::vec3 C = obb.position;	// OBB Center
		glm::vec3 E = obb.size;		// OBB Extents
		const glm::mat3 & o = obb.orientation;
		glm::vec3 A[] = {			// OBB Axis
				glm::vec3(o[0][0], o[0][1], o[0][2]),
				glm::vec3(o[1][0], o[1][1], o[1][2]),
				glm::vec3(o[2][0], o[2][1], o[2][2]),
		};

		v[0] = C + A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
		v[1] = C - A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
		v[2] = C + A[0] * E[0] - A[1] * E[1] + A[2] * E[2];
		v[3] = C + A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
		v[4] = C - A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
		v[5] = C + A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
		v[6] = C - A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
		v[7] = C - A[0] * E[0] - A[1] * E[1] + A[2] * E[2];

		return v;
	}

	std::vector<Line> getEdges(const OBB & obb)
	{
		std::vector<Line> result;
		result.reserve(12);
		std::vector<glm::vec3> v = getVertices(obb);

		int index[][2] = {
				{ 6, 1 },{ 6, 3 },{ 6, 4 },{ 2, 7 },{ 2, 5 },{ 2, 0 },
				{ 0, 1 },{ 0, 3 },{ 7, 1 },{ 7, 4 },{ 4, 5 },{ 5, 3 }
		};

		for (int j = 0; j < 12; ++j)
		{
			result.push_back(Line(
					v[index[j][0]], v[index[j][1]]
			));
		}

		return result;
	}

	float penetrationDepth(const OBB& o1, const OBB& o2, const glm::vec3 & axis, bool* outShouldFlip)
	{
		Interval i1 = getInterval(o1, glm::normalize(axis));
		Interval i2 = getInterval(o2, glm::normalize(axis));

		if (!((i2.min <= i1.max) && (i1.min <= i2.max)))
			return 0.0f;

		float len1 = i1.max - i1.min;
		float len2 = i2.max - i2.min;
		float min = fminf(i1.min, i2.min);
		float max = fmaxf(i1.max, i2.max);
		float length = max - min;

		if (outShouldFlip != 0) {
			*outShouldFlip = (i2.min < i1.min);
		}

		return (len1 + len2) - length;
	}

	std::vector<Plane> getPlanes(const OBB& obb)
	{
		glm::vec3 c = obb.position;	// OBB Center
		glm::vec3 e = obb.size;		// OBB Extents
		const glm::mat3 & o = obb.orientation;
		glm::vec3 a[] = {			// OBB Axis
				glm::vec3(o[0][0], o[0][1], o[0][2]),
				glm::vec3(o[1][0], o[1][1], o[1][2]),
				glm::vec3(o[2][0], o[2][1], o[2][2]),
		};

		std::vector<Plane> result;
		result.resize(6);

		result[0] = Plane(a[0]        ,  glm::dot(a[0], (c + a[0] * e.x)));
		result[1] = Plane(a[0] * -1.0f, -glm::dot(a[0], (c - a[0] * e.x)));
		result[2] = Plane(a[1]        ,  glm::dot(a[1], (c + a[1] * e.y)));
		result[3] = Plane(a[1] * -1.0f, -glm::dot(a[1], (c - a[1] * e.y)));
		result[4] = Plane(a[2]        ,  glm::dot(a[2], (c + a[2] * e.z)));
		result[5] = Plane(a[2] * -1.0f, -glm::dot(a[2], (c - a[2] * e.z)));

		return result;
	}

	bool clipToPlane(const Plane& plane, const Line& line, glm::vec3 * outPoint)
	{
		glm::vec3 ab = line.end - line.start;

		float nA = glm::dot(plane.normal, line.start);
		float nAB = glm::dot(plane.normal, ab);

		if (CMP(nAB, 0))
			return false;

		float t = (plane.distance - nA) / nAB;
		if (t >= 0.0f && t <= 1.0f) {
			if (outPoint != 0)
				*outPoint = line.start + ab * t;
			return true;
		}

		return false;
	}

	bool pointInOBB(const glm::vec3 & point, const OBB& obb)
	{
		glm::vec3 dir = point - obb.position;

		for (int i = 0; i < 3; ++i) {
			const glm::mat3 & orientation = obb.orientation;
			glm::vec3 axis(orientation[i][0], orientation[i][1], orientation[i][2]);

			float distance = glm::dot(dir, axis);

			if (distance > obb.size[i]) {
				return false;
			}
			if (distance < -obb.size[i]) {
				return false;
			}
		}

		return true;
	}

	std::vector<glm::vec3> clipEdgesToOBB(const std::vector<Line>& edges, const OBB& obb)
	{
		std::vector<glm::vec3> result;
		result.reserve(edges.size() * 3);
		glm::vec3 intersection;

		std::vector<Plane> planes = getPlanes(obb);

		for (int i = 0; i < planes.size(); ++i) {
			for (int j = 0; j < edges.size(); ++j) {
				if (clipToPlane(planes[i], edges[j], &intersection)) {
					if (pointInOBB(intersection, obb)) {
						result.push_back(intersection);
					}
				}
			}
		}

		return result;
	}

	CollisionManifold findCollisionFeatures(const Sphere& A, const Sphere& B)
	{
		CollisionManifold result; // Will return result of intersection!
		result.reset();

		float r = A.radius + B.radius;
		glm::vec3 d = B.position - A.position;

		if (glm::length2(d) - r * r > 0 || glm::length2(d) == 0.0f)
			return result;
		d = glm::normalize(d);

		result.colliding = true;
		result.normal = d;
		result.depth = fabsf(glm::length(d) - r) * 0.5f;

		// dtp - Distance to intersection point
		float dtp = A.radius - result.depth;
		glm::vec3 contact = A.position + d * dtp;

		result.contacts.push_back(contact);

		return result;
	}

	CollisionManifold findCollisionFeatures(const OBB& A, const Sphere& B)
	{
		CollisionManifold result; // Will return result of intersection!
		result.reset();

		glm::vec3 closestPt = closestPoint(A, B.position);

		float distanceSq = glm::length2(closestPt - B.position);
		if (distanceSq > B.radius * B.radius)
			return result;

		glm::vec3 normal;
		if (CMP(distanceSq, 0.0f)) {
			if (CMP(glm::length2(closestPt - A.position), 0.0f))
				return result;
			// Closest point is at the center of the sphere
			normal = glm::normalize(closestPt - A.position);
		}
		else
			normal = glm::normalize(B.position - closestPt);

		glm::vec3 outsidePoint = B.position - normal * B.radius;

		float distance = glm::length(closestPt - outsidePoint);

		result.colliding = true;
		result.contacts.push_back(closestPt + (outsidePoint - closestPt) * 0.5f);
		result.normal = normal;
		result.depth = distance * 0.5f;

		return result;
	}

	CollisionManifold findCollisionFeatures(const OBB& A, const OBB& B)
	{
		CollisionManifold result; // Will return result of intersection!
		result.reset();

		Sphere s1(A.position, glm::length(A.size));
		Sphere s2(B.position, glm::length(B.size));

		if (!sphereSphere(s1, s2))
			return result;

		const glm::mat3 & o1 = A.orientation;
		const glm::mat3 & o2 = A.orientation;

//		const float* o1 = A.orientation.asArray;
//		const float* o2 = B.orientation.asArray;

		glm::vec3 test[15] = {
				glm::vec3(o1[0][0], o1[0][1], o1[0][2]),
				glm::vec3(o1[1][0], o1[1][1], o1[1][2]),
				glm::vec3(o1[2][0], o1[2][1], o1[2][2]),
				glm::vec3(o2[0][0], o2[0][1], o2[0][2]),
				glm::vec3(o2[1][0], o2[1][1], o2[1][2]),
				glm::vec3(o2[2][0], o2[2][1], o2[2][2]),
		};

		for (int i = 0; i < 3; ++i) { // Fill out rest of axis
			test[6 + i * 3 + 0] = glm::cross(test[i], test[0]);
			test[6 + i * 3 + 1] = glm::cross(test[i], test[1]);
			test[6 + i * 3 + 2] = glm::cross(test[i], test[2]);
		}

		glm::vec3 * hitNormal = 0;
		bool shouldFlip;

		for (int i = 0; i < 15; ++i) {
			if (test[i].x < 0.000001f) test[i].x = 0.0f;
			if (test[i].y < 0.000001f) test[i].y = 0.0f;
			if (test[i].z < 0.000001f) test[i].z = 0.0f;
			if (glm::length2(test[i])< 0.001f)
				continue;

			float depth = penetrationDepth(A, B, test[i], &shouldFlip);
			if (depth <= 0.0f)
				return result;

			else if (depth < result.depth) {
				if (shouldFlip)
					test[i] = test[i] * -1.0f;
				result.depth = depth;
				hitNormal = &test[i];
			}
		}

		if (hitNormal == 0)
			return result;
		glm::vec3 axis = glm::normalize(*hitNormal);

		std::vector<glm::vec3> c1 = clipEdgesToOBB(getEdges(B), A);
		std::vector<glm::vec3> c2 = clipEdgesToOBB(getEdges(A), B);
		result.contacts.reserve(c1.size() + c2.size());
		result.contacts.insert(result.contacts.end(), c1.begin(), c1.end());
		result.contacts.insert(result.contacts.end(), c2.begin(), c2.end());

		Interval i = getInterval(A, axis);
		float distance = (i.max - i.min)* 0.5f - result.depth * 0.5f;
		glm::vec3 pointOnPlane = A.position + axis * distance;

		for (int i = result.contacts.size() - 1; i >= 0; --i) {
			glm::vec3 contact = result.contacts[i];
			result.contacts[i] = contact + (axis * glm::dot(axis, pointOnPlane - contact));

			// This bit is in the "There is more" section of the book
			for (int j = result.contacts.size() - 1; j > i; --j) {
				if (glm::length2(result.contacts[j] - result.contacts[i]) < 0.0001f) {
					result.contacts.erase(result.contacts.begin() + j);
					break;
				}
			}
		}

		result.colliding = true;
		result.normal = axis;

		return result;
	}
}
