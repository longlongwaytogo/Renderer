//11.1.anti_aliasing.cpp
#include<TestOpenGLBase.h>
#include <box.h>
using namespace RenderSystem;
class anti_aliasing :public TestBase
{

	virtual bool InitGLFW()
	{
		glfwInit();
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// glfw window creation

		if (m_windowName.empty())
		{
			m_windowName = "LearnOpenGL";
		}
		m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, m_windowName.c_str(), NULL, NULL);
		glfwMakeContextCurrent(m_window);
		if (m_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
		glfwSetCursorPosCallback(m_window, mouse_callback);
		glfwSetScrollCallback(m_window, scroll_callback);
		glfwSetKeyCallback(m_window, keyboard_callback);

		// tell GLFW to capture our mouse
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
	}


	virtual void InitShader()
	{
		TestBase::InitShader();
	}
	virtual void InitData()
	{
		TestBase::InitData();
	}
	virtual void Render()
	{
		//glEnable(GL_MULTISAMPLE);
		TestBase::Render();
	}
private:
};
START_TEST(anti_aliasing)
