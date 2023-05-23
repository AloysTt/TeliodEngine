#include <physics/Rigidbody.h>
#include "glm/gtx/norm.hpp"

namespace teliod::physics
{
	CollisionManifold findCollisionFeatures(Rigidbody & _ra, Rigidbody & _rb)
	{
		if (!_ra.hasVolume() || !_rb.hasVolume())
			return {};

		RigidbodyImplVolume & ra = *static_cast<RigidbodyImplVolume*>(_ra.getImpl());
		RigidbodyImplVolume & rb = *static_cast<RigidbodyImplVolume*>(_rb.getImpl());


		CollisionManifold result;
		result.reset();

		if (ra.getType() == RigidbodyType::SPHERE) {
			if (rb.getType() == RigidbodyType::SPHERE)
				result = findCollisionFeatures(ra.sphere, rb.sphere);
			else if (rb.getType() == RigidbodyType::BOX)
			{
				result = findCollisionFeatures(rb.box, ra.sphere);
				result.normal = result.normal * -1.0f;
			}
		}
		else if (ra.getType() == RigidbodyType::BOX)
		{
			if (rb.getType() == RigidbodyType::BOX)
				result = findCollisionFeatures(ra.box, rb.box);
			else if (rb.getType() == RigidbodyType::SPHERE)
				result = findCollisionFeatures(ra.box, rb.sphere);
		}

		return result;
	}

	void applyImpulse(Rigidbody & _A, Rigidbody & _B, const CollisionManifold & M, int c)
	{
		if (!_A.hasVolume() || !_B.hasVolume())
			return;

		RigidbodyImplVolume & A = *static_cast<RigidbodyImplVolume*>(_A.getImpl());
		RigidbodyImplVolume & B = *static_cast<RigidbodyImplVolume*>(_B.getImpl());

		// Linear impulse
		float invMass1 = A.invMass();
		float invMass2 = B.invMass();
		float invMassSum = invMass1 + invMass2;

		if (invMassSum == 0.0f)
			return;

		glm::vec3 r1 = M.contacts[c] - A.position;
		glm::vec3 r2 = M.contacts[c] - B.position;
		glm::mat4 i1 = A.invTensor();
		glm::mat4 i2 = B.invTensor();

		// Relative velocity
		glm::vec3 relativeVel = (B.velocity + glm::cross(B.angVel, r2)) - (A.velocity + glm::cross(A.angVel, r1));

		// Relative collision normal
		glm::vec3 relativeNorm = M.normal;
		relativeNorm = glm::normalize(relativeNorm);

		// Moving away from each other? Do nothing!
		if (glm::dot(relativeVel, relativeNorm) > 0.0f)
			return;

		float e = std::fminf(A.cor, B.cor);

		float numerator = (-(1.0f + e) * glm::dot(relativeVel, relativeNorm));
		float d1 = invMassSum;
		glm::vec3 d2 = glm::cross(glm::vec3(glm::vec4(glm::cross(r1, relativeNorm), 0.0f) * i1), r1);
		glm::vec3 d3 = glm::cross(glm::vec3(glm::vec4(glm::cross(r2, relativeNorm), 0.0f) * i2), r2);
		float denominator = d1 + glm::dot(relativeNorm, d2 + d3);

		float j = (denominator == 0.0f) ? 0.0f : numerator / denominator;
		if (M.contacts.size() > 0.0f && j != 0.0f)
			j /= (float)M.contacts.size();

		glm::vec3 impulse = relativeNorm * j;
		A.velocity = A.velocity - impulse *  invMass1;
		B.velocity = B.velocity + impulse *  invMass2;

		A.angVel = A.angVel - glm::vec3(glm::vec4(glm::cross(r1, impulse), 0.0f) * i1);
		B.angVel = B.angVel + glm::vec3(glm::vec4(glm::cross(r2, impulse), 0.0f) * i2);

		// Friction
		glm::vec3 t = relativeVel - (relativeNorm * glm::dot(relativeVel, relativeNorm));
		if (CMP(glm::length2(t), 0.0f))
			return;
		t = glm::normalize(t);

		numerator = -glm::dot(relativeVel, t);
		d1 = invMassSum;
		d2 = glm::cross(glm::vec3(glm::vec4(glm::cross(r1, t), 0.0f) * i1), r1);
		d3 = glm::cross(glm::vec3(glm::vec4(glm::cross(r2, t), 0.0f) * i2), r2);
		denominator = d1 + glm::dot(t, d2 + d3);

		float jt = (denominator == 0.0f) ? 0.0f : numerator / denominator;
		if (M.contacts.size() > 0.0f && jt != 0.0f)
			jt /= (float)M.contacts.size();

		if (CMP(jt, 0.0f))
			return;

		glm::vec3 tangentImpuse;
#ifdef DYNAMIC_FRICTION
		float sf = sqrtf(A.staticFriction * B.staticFriction);
	float df = sqrtf(A.dynamicFriction * B.dynamicFriction);
	if (fabsf(jt) < j * sf) {
		tangentImpuse = t * jt;
	}
	else {
		tangentImpuse = t * -j * df;
	}
#else
		float friction = sqrtf(A.friction * B.friction);
		if (jt > j * friction) {
			jt = j * friction;
		}
		else if (jt < -j * friction) {
			jt = -j * friction;
		}
		tangentImpuse = t * jt;
#endif

		A.velocity = A.velocity - tangentImpuse *  invMass1;
		B.velocity = B.velocity + tangentImpuse *  invMass2;

		A.angVel = A.angVel - glm::vec3(glm::vec4(glm::cross(r1, tangentImpuse), 0.0f) * i1);
		B.angVel = B.angVel + glm::vec3(glm::vec4(glm::cross(r2, tangentImpuse), 0.0f) * i2);
	}
}
