#ifndef	TELIOD_PHYSICS_RIGIDBODY_H
#define	TELIOD_PHYSICS_RIGIDBODY_H

#include <vector>
#include <physics/Geometry.h>
#include <glm/vec3.hpp>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "CollisionManifold.h"

namespace teliod::physics
{

    enum class RigidbodyType
    {
        BOX,
        SPHERE,
    };

	class RigidbodyImpl
	{
	public:
		inline RigidbodyImpl(RigidbodyType _type)
        : type(_type)
        {
        }
		virtual inline ~RigidbodyImpl() = default;

		virtual inline void update(float dt) {}
		virtual inline void applyForces() {}
		virtual inline void solveConstraints(const std::vector<OBB> & constraints) {}
		virtual inline RigidbodyImpl * clone() = 0;

        inline RigidbodyType getType() const
        {
            return type;
        }

        inline bool hasVolume() const
        {
            return type == RigidbodyType::SPHERE || type == RigidbodyType::BOX;
        }

    private:
        RigidbodyType type;
	};

	// wrapper component
	class Rigidbody
	{
	public:
		inline Rigidbody()
		: pImpl(nullptr)
		{
		}

        inline Rigidbody(Rigidbody && other)
        : pImpl(nullptr)
        {
            std::swap(pImpl, other.pImpl);
        }
        inline Rigidbody & operator=(Rigidbody && other)
        {
            std::swap(pImpl, other.pImpl);
            return *this;
        }

        inline Rigidbody(const Rigidbody & other)
        : pImpl(other.pImpl->clone())
        {
        }

        inline Rigidbody & operator=(const Rigidbody & other)
        {
            if (pImpl != nullptr)
                delete pImpl;
            pImpl = other.pImpl->clone();
            return *this;
        }

		inline ~Rigidbody()
		{
            if (pImpl != nullptr)
			    delete pImpl;
		}

        inline void setImpl(RigidbodyImpl * _pImpl)
        {
            pImpl = _pImpl;
        }

		inline void update(float dt)
		{
			pImpl->update(dt);
		}

		inline void applyForces()
		{
			pImpl->applyForces();
		}

		inline void solveConstraints(const std::vector<OBB> & constraints)
		{
			pImpl->solveConstraints(constraints);
		}

        inline bool hasVolume() const
        {
            return pImpl->hasVolume();
        }

        inline RigidbodyImpl * getImpl()
        {
            return pImpl;
        }


	private:
		RigidbodyImpl * pImpl;
	};


	// rigidbody types


	class RigidbodyImplVolume : public RigidbodyImpl {
	public:
		inline RigidbodyImplVolume()
		: RigidbodyImpl(RigidbodyType::BOX)
        , position(0.0f, 0.0f, 0.0f)
        , velocity(0.0f, 0.0f, 0.0f)
        , orientation(0.0f, 0.0f, 0.0f)
        , angVel(0.0f, 0.0f, 0.0f)
        , forces(0.0f, 0.0f, 0.0f)
        , torques(0.0f, 0.0f, 0.0f)
        , cor(0.5f)
		, mass(1.0f)
#ifdef DYNAMIC_FRICTION
		, staticFriction(0.5f)
		, dynamicFriction(0.3f)
#else
		, friction(0.6f)
#endif
		{
		}

		inline RigidbodyImplVolume(RigidbodyType bodyType)
        : RigidbodyImpl(bodyType)
        , cor(0.5f)
        , mass(1.0f)
#ifdef DYNAMIC_FRICTION
        , staticFriction(0.5f)
		, dynamicFriction(0.3f)
#else
        , friction(0.6f)
#endif
        {
        }

		inline virtual ~RigidbodyImplVolume() { }

		inline virtual void update(float dt); // update position

		inline float invMass() const;
		inline glm::mat4 invTensor() const;

		inline virtual void applyForces();
		inline void syncCollisionVolumes();

		inline virtual void addLinearImpulse(const glm::vec3 & impulse);
		inline virtual void addRotationalImpulse(const glm::vec3 & point, const glm::vec3 & impulse);


        virtual inline RigidbodyImpl * clone()
        {
            RigidbodyImplVolume * pNew = new RigidbodyImplVolume();
            pNew->position = position;
            pNew->velocity = velocity;
            pNew->orientation = orientation;
            pNew->angVel = angVel;

            pNew->forces = forces;
            pNew->torques = torques;

            pNew->mass = mass;
            pNew->cor = cor;
            pNew->friction = friction;

            pNew->box = box;
            pNew->sphere = sphere;
            return pNew;
        };

	public:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 orientation;
		glm::vec3 angVel;

		glm::vec3 forces; // sum forces
		glm::vec3 torques; // sum torques

		//vec3 inertia;
		float mass;
		float cor; // Coefficient of restitution
#ifdef DYNAMIC_FRICTION
		float staticFriction;
	float dynamicFriction;
#else
		float friction;
#endif

		OBB box;
		Sphere sphere;
	};

    CollisionManifold findCollisionFeatures(Rigidbody & _ra, Rigidbody & _rb);

    void applyImpulse(Rigidbody & _A, Rigidbody & _B, const CollisionManifold & M, int c);
}

#include <physics/Rigidbody.inl>

#endif // TELIOD_PHYSICS_RIGIDBODY_H
