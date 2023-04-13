#ifndef TELIOD_RENDER_MESHRENDERERSYSTEM_H
#define TELIOD_RENDER_MESHRENDERERSYSTEM_H

#include <ECS/ECS.h>

namespace teliod::render
{
	class MeshRendererSystem : public ecs::System
	{
	public:
		void render();
	};
}

#endif // TELIOD_RENDER_MESHRENDERERSYSTEM_H
