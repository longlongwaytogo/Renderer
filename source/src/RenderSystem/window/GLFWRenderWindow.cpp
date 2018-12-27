
#include <RenderSystem/window/GLFWRenderWindow.h>
#include <comm/glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <comm/camera.h>

#include <RenderSystem/System.h>
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

	void* GLFWRenderWindow::GetNativeWindowHandle()
	{
		return static_cast<void*>(m_nativeWindow);
	}

	bool GLFWRenderWindow::Init()
	{
		bool ret = InitWindow();
		ret &= InitGL();
		return ret;
	}

	bool GLFWRenderWindow::InitWindow()
	{
		// 1. set opengl profile
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_nativeWindow = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);
		if (!m_nativeWindow)
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
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to load OpenGL Function !" << std::endl;
			return -1;

		}
		return false;
	}

	void GLFWRenderWindow::OnSize(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void GLFWRenderWindow::OnMouse(double xpos, double ypos)
	{
		
		static float lastX = (float)m_width / 2.0;
		static float lastY = (float)m_height / 2.0;
		static bool firstMouse = true;

		// timing
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		GetSystem().GetRenderStage()->GetCmaera().ProcessMouseMovement(xoffset, yoffset);
	}

	void GLFWRenderWindow::OnScroll(double xoffset, double yoffset)
	{
		GetSystem().GetRenderStage()->GetCmaera().ProcessMouseScroll(yoffset); 
	}

	void GLFWRenderWindow::OnProcessInput(float deltaTime)
	{
		if (glfwGetKey(m_nativeWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(m_nativeWindow, true);
			GetSystem().GetRenderStage()->Stop();
		}
		if (glfwGetKey(m_nativeWindow, GLFW_KEY_W) == GLFW_PRESS)
			GetSystem().GetRenderStage()->GetCmaera().ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(m_nativeWindow, GLFW_KEY_S) == GLFW_PRESS)
			GetSystem().GetRenderStage()->GetCmaera().ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(m_nativeWindow, GLFW_KEY_A) == GLFW_PRESS)
			GetSystem().GetRenderStage()->GetCmaera().ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(m_nativeWindow, GLFW_KEY_D) == GLFW_PRESS)
			GetSystem().GetRenderStage()->GetCmaera().ProcessKeyboard(RIGHT, deltaTime);

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

}
