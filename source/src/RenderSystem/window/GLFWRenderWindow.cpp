
#include <RenderSystem/window/GLFWRenderWindow.h>

#include <glfw/glfw3.h>
#include <iostream>
namespace RenderSystem
{
	GLFWRenderWindow* GLFWRenderWindow::s_RenderWindow = nullptr;
	GLFWRenderWindow::GLFWRenderWindow():m_nativeWindow(nullptr)
	{
		s_RenderWindow = this;
	}

	GLFWRenderWindow::~GLFWRenderWindow()
	{
	}

	void GLFWRenderWindow::SetWindowSize(int w, int h)
	{
		m_width = w;
		m_height = h;
	}

	bool GLFWRenderWindow::InitWindow()
	{
		// 1. set opengl profile
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_nativeWindow = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);
		if (m_nativeWindow)
		{
			std::cout << "Fail to create GLFW window." << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(m_nativeWindow);
		glfwSetFramebufferSizeCallback(m_nativeWindow, &OnSizeCallback);
		glfwSetCursorPosCallback(m_nativeWindow, &OnMouseCallback);
		glfwSetScrollCallback(m_nativeWindow, &OnScrollCallback);

		// tell GLFW to capture our mouse
		glfwSetInputMode(m_nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		return true;
	}

	bool GLFWRenderWindow::InitGL()
	{
		// glad laod all OpenGL function pointers
	/*	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to load OpenGL Function !" << std::endl;
			return -1;

		}*/
		return false;
	}

	void GLFWRenderWindow::OnSize(int width, int height)
	{

	}

	void GLFWRenderWindow::OnMouse(double xpos, double ypos)
	{

	}

	void GLFWRenderWindow::OnScroll(double xoffset, double yoffset)
	{

	}

	void GLFWRenderWindow::OnProcessInput()
	{

	}

//////////////////////////////////////////////////////////////////////////
// static callback
//////////////////////////////////////////////////////////////////////////
	void OnSizeCallback(GLFWwindow* window, int width, int height)
	{
		GLFWRenderWindow::s_RenderWindow->OnSize(width, height);
	}

	void OnMouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		GLFWRenderWindow::s_RenderWindow->OnMouse(xpos, ypos);
	}

	void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		GLFWRenderWindow::s_RenderWindow->OnScroll(xoffset, yoffset);
	}

	void OnProcessInputCallback(GLFWwindow* window)
	{
		GLFWRenderWindow::s_RenderWindow->OnProcessInput();
	}

}
