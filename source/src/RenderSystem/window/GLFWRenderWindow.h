//////////////////////////////////////////////////////////////////////////
//@file: GLFWWindow.h
//@brief: GLFWʵ�ִ���ϵͳ��ʾ
//@author: szh
//@date: 2018/12/25

#ifndef RENDERER_INCLUDE_RENDERSYSTEM_GLFWRENDERWINDOW_H
#define RENDERER_INCLUDE_RENDERSYSTEM_GLFWRENDERWINDOW_H

#include <RenderSystem/WindowBase.h>
class GLFWwindow;
namespace RenderSystem
{

	static void OnSizeCallback(GLFWwindow* window, int width, int height);
	static void OnMouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
 
	class GLFWRenderWindow :public WindowBase
	{
	public:
		GLFWRenderWindow();
		~GLFWRenderWindow();

		virtual void SetWindowSize(int w, int h);

		virtual void* GetNativeWindowHandle();

		virtual bool Init();

		virtual bool InitWindow();

		bool InitGL();

		/*
		SetUpWindow �������úü��������󣬴�������ϵͳ
		*/

		void OnSize(int width, int height);
		void OnMouse( double xpos, double ypos);
		void OnScroll(double xoffset, double yoffset);
		void OnProcessInput(float deltaTime);
	private:
		GLFWwindow* m_nativeWindow;
	public:
		static GLFWRenderWindow* s_RenderWindow;
	};
}
#endif // RENDERER_INCLUDE_RENDERSYSTEM_GLFWRENDERWINDOW_H