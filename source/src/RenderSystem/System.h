//////////////////////////////////////////////////////////////////////////
//@file: System.h
//@brief: 整个渲染系统的入口点
//@description:
//@author: szh
//@date: 2018/12/25

#ifndef RENDER_INCLUDE_RENDERSYSTEM_SYSTEM_H
#define RENDER_INCLUDE_RENDERSYSTEM_SYSTEM_H
#include <RenderSystem/Export.h>
#include <RenderSystem/RenderWindow.h>
#include <RenderSystem/RenderStage.h>
namespace RenderSystem
{
	class RENDERSYSTEM_API System
	{
	public:
		static System& GetInstance();
		RenderWindowPtr GetRenderWindow();

		RenderStagePtr GetRenderStage();
		bool Init();

		void Run();

		void Terminate();
		~System();

	protected:
		 
		 
	private:
		System();
		RenderWindowPtr m_window;
		RenderStagePtr m_renderStage;
	};
}
#define GetSystem() RenderSystem::System::GetInstance()

#endif // RENDER_INCLUDE_RENDERSYSTEM_SYSTEM_H