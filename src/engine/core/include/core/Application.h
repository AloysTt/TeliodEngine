#ifndef TELIOD_CORE_APPLICATION_H
#define TELIOD_CORE_APPLICATION_H

namespace teliod::render
{
	class MeshRendererSystem;
}

namespace teliod::core
{
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

		virtual void runInternal() = 0;
	private:

	public:
	protected:
		bool shouldClose;
	private:
		render::MeshRendererSystem * pMeshRendererSystem;
	};
}

#endif // TELIOD_CORE_APPLICATION_H
