//9.1.GeometryShader.cpp
#include<TestOpenGLBase.h>
#include <box.h>
using namespace RenderSystem;

float points[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
	0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
	0.5f, -0.5f, 1.0f, 0.0f, 1.0f, // bottom-right
	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
};

class GeometryShader :public TestBase
{

	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");
		m_lineShader.loadFile((shaderPath + "9.1.GeometryShader.vs").c_str(), (shaderPath + "9.1.GeometryShader.ps").c_str(),
			(shaderPath + "9.1.GeometryShader_1.gs").c_str());
		m_houseShader.loadFile((shaderPath + "9.1.GeometryShader.vs").c_str(), (shaderPath + "9.1.GeometryShader.ps").c_str(),
			(shaderPath + "9.1.GeometryShader_2.gs").c_str());
	}
	virtual void InitData()
	{
		TestBase::InitData();

		glGenVertexArrays(1, &m_lineVAO);
		glBindVertexArray(m_lineVAO);
		glGenBuffers(1, &m_lineVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 + 2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		m_index = 0;

	}

	virtual void OnKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{/*
			[in]	window	The window that received the event.
			[in]	key	The keyboard key that was pressed or released.
			[in]	scancode	The system-specific scancode of the key.
			[in]	action	GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
			[in]	mods	Bit field describing which modifier keys were held down.
			*/
		if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
		{
			m_index++;
			if (m_index > 1)
				m_index = 0;
		}

	}
	virtual void Render()
	{
		//TestBase::Render();

		if (m_index == 0)
			m_lineShader.use();
		else
			m_houseShader.use();

		glBindVertexArray(m_lineVAO);
		glDrawArrays(GL_POINTS, 0, 4);
	}
private:

	unsigned int m_lineVAO;
	unsigned int m_lineVBO;
	unsigned int m_index; 
	Shader m_lineShader;
	Shader m_houseShader;
};
START_TEST(GeometryShader)

