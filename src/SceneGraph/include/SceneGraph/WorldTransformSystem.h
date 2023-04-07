#ifndef TELIOD_SG_WORLDTRANSFORMSYSTEM_H
#define TELIOD_SG_WORLDTRANSFORMSYSTEM_H

#include "ECS/ECS.h"
#include "SceneGraph.h"
#include "glm/mat4x4.hpp"

namespace teliod::sg::systems
{
	class WorldTransformSystem : public ecs::System
	{
	public:
		void update();
	private:
		void recurseUpdateWorldTF(glm::mat4x4 mat, Node * n);
	};
}

#endif // TELIOD_SG_WORLDTRANSFORMSYSTEM_H
