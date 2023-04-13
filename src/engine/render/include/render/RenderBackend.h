#ifndef TELIOD_RENDER_RENDERBACKEND_H
#define TELIOD_RENDER_RENDERBACKEND_H


class GLFWwindow;

namespace teliod::render
{
	class RenderBackend
	{
	public:
		static RenderBackend & getInstance();

		void init();
		void destroy();
		void preFrameUpdate();
		void postFrameUpdate();

		bool windowShouldClose() const;
	private:
		GLFWwindow * window;
	};
}

#endif // TELIOD_RENDER_RENDERBACKEND_H
