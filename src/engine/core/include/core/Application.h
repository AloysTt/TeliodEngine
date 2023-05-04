#ifndef TELIOD_CORE_APPLICATION_H
#define TELIOD_CORE_APPLICATION_H

namespace teliod::render
{
	class MeshRendererSystem;
}

namespace teliod::sg::systems
{
	class WorldTransformSystem;
}

namespace teliod::core
{
	class CameraSystem;

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void init();
		void destroy();

		void run();
	protected:
		virtual void initInternal() = 0;
		virtual void destroyInternal() = 0;

		virtual void runInternal(float dt) = 0;
	private:

	public:
	protected:
		bool shouldClose;
	private:
		render::MeshRendererSystem * pMeshRendererSystem;
		sg::systems::WorldTransformSystem * pWorldTransformSystem;
		core::CameraSystem * pCameraSystem;
	};
}

#endif // TELIOD_CORE_APPLICATION_H
