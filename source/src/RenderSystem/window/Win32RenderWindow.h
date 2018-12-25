//////////////////////////////////////////////////////////////////////////
//@file: GLFWWindow.h
//@brief: GLFW实现窗口系统显示
//@author: szh
//@date: 2018/12/25

#ifndef RENDERER_INCLUDE_RENDERSYSTEM_WIN32_RENDERWINDOW_H
#define RENDERER_INCLUDE_RENDERSYSTEM_WIN32_RENDERWINDOW_H

#include <RenderSystem/WindowBase.h>
namespace RenderSystem
{
	class Win32RenderWindow :public WindowBase
	{
	public:
		Win32RenderWindow();
		~Win32RenderWindow();

		virtual void SetUpWindow(int mode);
	private:
		 
	};

	

}
#endif // RENDERER_INCLUDE_RENDERSYSTEM_WIN32_RENDERWINDOW_H