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
			;// m_windowImpl = std::shared_ptr<WindowBase>(new Win32RenderWindow);
		m_windowImpl->SetWindowName("Render");
		m_windowImpl->SetWindowSize(1024, 768);
		return m_windowImpl->Init();
	}

	void RenderWindow::GetWindowSize(int& w, int& h)
	{
		m_windowImpl->GetWindowSize(w, h);
	}

	void RenderWindow::UpdateEvent(float deltaTime)
	{
		m_windowImpl->OnProcessInput(deltaTime);
	}

	RenderWindow::RenderWindow() :m_mode(WINDOW_GLFW)
	{
		
	}

	RenderWindow::~RenderWindow()
	{

	}

	void* RenderWindow::GetNativeWindowHandle()
	{
		return m_windowImpl->GetNativeWindowHandle();
	}

}

