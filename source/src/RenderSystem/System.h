//////////////////////////////////////////////////////////////////////////
//@file: System.h
//@brief: ������Ⱦϵͳ����ڵ�
//@description:
//@author: szh
//@date: 2018/12/25

#include <RenderSystem/Export.h>
#include <RenderSystem/RenderWindow.h>
namespace RenderSystem
{
	class RENDERSYSTEM_API System
	{
	public:
		static System& GetInstance();
		RenderWindowPtr GetRenderWindow();
		bool Init();
		~System();
	private:
		System();
		RenderWindowPtr m_window;
	};

#define GetRS() RenderSystem::System::GetInstance();
}
