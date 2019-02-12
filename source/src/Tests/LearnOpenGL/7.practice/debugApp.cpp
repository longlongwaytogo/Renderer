//debugApp.cpp
#include<TestOpenGLBase.h>
#include <debugger.h>
#include <RenderSystem/scene/Box.h>

using namespace RenderSystem;
class debugApp :public TestBase
{

public:

	virtual bool InitGLFW()
	{
		// glfw: initialize and configure
		// ------------------------------

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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

		// due to a bug in GLEW the glewInit call always generates an OpenGL error; clear the flag(s) by calling glGetError();
		glGetError();
		// setup debugger
		Debugger::setUpDebugInfo();
	}


	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string path = FileSystem::getPath("resources\\shaders\\tests\\LearnOpenGL\\Shaders\\7.practice\\");
		m_debuggerShader.loadFile(path + "debugApp.vs", path + "debugApp.ps");
		m_debuggerShader.use();
		m_debuggerShader.setInt("tex", 0);
		
	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_box.Init();
		m_tex = loadTexture(FileSystem::getPath("resources/textures/container2.png"));
		m_camera.SetPose(glm::vec3(0, 0, 3));
	}
	
	virtual void PreRender()
	{
		__super::PreRender();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		// glFrontFace(GL_CW); // 使用顺时针为正面
	}
	virtual void Render()
	{
		TestBase::Render();

		m_debuggerShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		m_debuggerShader.setMat4("projection", projection);
		m_debuggerShader.setMat4("view", m_camera.GetViewMatrix());

		GLfloat rotationSpeed = 10.0f;
		GLfloat angle = (float)glfwGetTime() * rotationSpeed;
		glm::mat4 model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
		m_debuggerShader.setMat4("model", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_tex);
		m_box.Draw();

		
	}
private:
	Shader m_debuggerShader;
	Box m_box;
	unsigned int m_tex;
	
};
START_TEST(debugApp)
