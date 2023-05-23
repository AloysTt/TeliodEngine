namespace teliod::physics
{
    namespace constants
    {
        constexpr glm::vec3 GRAVITY{0.0f, -9.82f, 0.0f};
    }

	inline void RigidbodyImplVolume::update(float dt)
	{
        // Integrate velocity
        const float damping = 0.98f;

        glm::vec3 acceleration = forces * invMass();
        velocity = velocity + acceleration * dt;
        velocity = velocity * damping;

        if (std::fabsf(velocity.x) < 0.001f)
            velocity.x = 0.0f;
        if (std::fabsf(velocity.y) < 0.001f)
            velocity.y = 0.0f;
        if (std::fabsf(velocity.z) < 0.001f)
            velocity.z = 0.0f;

        if (getType() == RigidbodyType::BOX)
        {
            glm::vec3 angAccel = glm::vec4(torques, 0.0f) * invTensor();
            angVel = angVel + angAccel * dt;
            angVel = angVel *  damping;

            if (std::fabsf(angVel.x) < 0.001f)
                angVel.x = 0.0f;
            if (std::fabsf(angVel.y) < 0.001f)
                angVel.y = 0.0f;
            if (std::fabsf(angVel.z) < 0.001f)
                angVel.z = 0.0f;
        }

        // Integrate position
        position = position + velocity * dt;

        if (getType() == RigidbodyType::BOX)
            orientation = orientation + angVel * dt;

        syncCollisionVolumes();
	}

    inline float RigidbodyImplVolume::invMass() const
    {
        if (mass == 0.0f)
            return 0.0f;
        return 1.0f / mass;
    }

    inline glm::mat4 RigidbodyImplVolume::invTensor() const
    {
        if (mass == 0)
            return glm::mat4{
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0
            };

        float ix = 0.0f;
        float iy = 0.0f;
        float iz = 0.0f;
        float iw = 0.0f;

        if (mass != 0 && getType() == RigidbodyType::SPHERE) {
            float r2 = sphere.radius * sphere.radius;
            float fraction = (2.0f / 5.0f);

            ix = r2 * mass * fraction;
            iy = r2 * mass * fraction;
            iz = r2 * mass * fraction;
            iw = 1.0f;
        }
        else if (mass != 0 && getType() == RigidbodyType::BOX) {
            glm::vec3 size = box.size * 2.0f;
            float fraction = (1.0f / 12.0f);

            float x2 = size.x * size.x;
            float y2 = size.y * size.y;
            float z2 = size.z * size.z;

            ix = (y2 + z2) * mass * fraction;
            iy = (x2 + z2) * mass * fraction;
            iz = (x2 + y2) * mass * fraction;
            iw = 1.0f;
        }

        return glm::inverse(glm::mat4{
                ix, 0, 0, 0,
                0, iy, 0, 0,
                0, 0, iz, 0,
                0, 0, 0, iw});
    }


    inline void RigidbodyImplVolume::applyForces()
    {
        forces = constants::GRAVITY * mass;
    }

    inline void RigidbodyImplVolume::syncCollisionVolumes()
    {
        sphere.position = position;
        box.position = position;

        box.orientation = glm::eulerAngleXYZ(
                orientation.x,
                orientation.y,
                orientation.z
        );

		box.orientationQuat = glm::quat(orientation);
    }

    inline void RigidbodyImplVolume::addLinearImpulse(const glm::vec3 & impulse)
    {
        velocity = velocity + impulse;
    }

    inline void RigidbodyImplVolume::addRotationalImpulse(const glm::vec3 & point, const glm::vec3 & impulse)
    {
        glm::vec3 centerOfMass = position;
        glm::vec3 torque = glm::cross(point - centerOfMass, impulse);

        glm::vec3 angAccel = glm::vec4(torque, 0.0f) * invTensor();
        angVel = angVel + angAccel;
    }
}
