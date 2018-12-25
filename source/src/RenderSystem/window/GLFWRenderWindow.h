//////////////////////////////////////////////////////////////////////////
//@file: GLFWWindow.h
//@brief: GLFW实现窗口系统显示
//@author: szh
//@date: 2018/12/25

#ifndef RENDERER_INCLUDE_RENDERSYSTEM_GLFWRENDERWINDOW_H
#define RENDERER_INCLUDE_RENDERSYSTEM_GLFWRENDERWINDOW_H

#include <RenderSystem/WindowBase.h>

#include <GLFW/glfw3.h>


namespace RenderSystem
{

	static void OnSizeCallback(GLFWwindow* window, int width, int height);
	static void OnMouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void OnProcessInputCallback(GLFWwindow* window);

	class GLFWRenderWindow :public WindowBase
	{
	public:
		GLFWRenderWindow();
		~GLFWRenderWindow();

		virtual void SetWindowSize(int w, int h);
		 
		virtual bool InitWindow();

		bool InitGL();
		/*
		SetUpWindow 用于设置好几本参数后，创建窗口系统
		*/

		void OnSize(int width, int height);
		void OnMouse( double xpos, double ypos);
		void OnScroll(double xoffset, double yoffset);
		void OnProcessInput();
	private:
		GLFWwindow* m_nativeWindow;
	public:
		static GLFWRenderWindow* s_RenderWindow;
	};
}
#endif // RENDERER_INCLUDE_RENDERSYSTEM_GLFWRENDERWINDOW_H