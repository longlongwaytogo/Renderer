//////////////////////////////////////////////////////////////////////////
//@file: RenderWindow.h
//@brief: 对外访问的窗口接口
//@author: szh
//@date: 2018/12/25

#ifndef RENDERER_SRC_RENDER_RENDERWINDOW_H
#define RENDERER_SRC_RENDER_RENDERWINDOW_H
#include <RenderSystem/Export.h>
#include <memory>
namespace RenderSystem
{
	class WindowBase;
	 class RENDERSYSTEM_API RenderWindow
	{
	public:
		enum {
			WINDOW_GLFW,
			WINDOW_WIN32,
		};
		bool CreateRenderWindow(int mode);
		RenderWindow();
		~RenderWindow();

	private:
		std::shared_ptr<WindowBase> m_windowImpl;
		int m_mode;
	};
	 typedef std::shared_ptr<RenderWindow> RenderWindowPtr;
}

#endif // RENDERER_SRC_RENDER_RENDERWINDOW_H