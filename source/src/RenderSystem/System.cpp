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

bool RenderSystem::System::Init()
{
	m_window = std::shared_ptr<RenderWindow>(new RenderWindow);
	return true;
}


RenderSystem::System::~System()
{

}

RenderSystem::System::System()
{

}
