#include <RenderSystem/RenderWindow.h>
#include <RenderSystem/window/GLFWRenderWindow.h>
#include <RenderSystem/window/Win32RenderWindow.h>

namespace RenderSystem
{
	bool RenderWindow::CreateRenderWindow(int mode)
	{
		m_mode = mode;
		if (m_mode == WINDOW_GLFW)
			m_windowImpl = std::shared_ptr<WindowBase>(new GLFWRenderWindow);
		else
			m_windowImpl = std::shared_ptr<WindowBase>(new Win32RenderWindow);
		return true;
	}

	RenderWindow::RenderWindow():m_mode(WINDOW_GLFW)
	{
		
	}

	RenderWindow::~RenderWindow()
	{

	}

}

