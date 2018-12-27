#include <RenderSystem/System.h>
#include <RenderSystem/RenderWindow.h>
 

RenderSystem::System& RenderSystem::System::GetInstance()
{
	static RenderSystem::System s_system;
	return s_system;
}

RenderSystem::RenderWindowPtr RenderSystem::System::GetRenderWindow()
{
	return m_window;
}

RenderSystem::RenderStagePtr RenderSystem::System::GetRenderStage()
{
	return m_renderStage;
}

bool RenderSystem::System::Init()
{
	
	m_window = std::make_shared<RenderWindow>();
	m_renderStage = std::make_shared<RenderStage>(m_window);
	bool ret = m_window->CreateRenderWindow(RenderWindow::WINDOW_GLFW);
	ret &= m_renderStage->Init();
	return ret;

	
}


void RenderSystem::System::Run()
{
	if (m_renderStage)
	{
		m_renderStage->RenderLoop();
	}
}

void RenderSystem::System::Terminate()
{
	m_renderStage->Stop();
}

RenderSystem::System::~System()
{

}

RenderSystem::System::System()
{

}
