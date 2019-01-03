//10.1.Instancing_quads.cpp
#include<TestOpenGLBase.h>
#include <box.h>
using namespace RenderSystem;
#define  INSTANCE_COUNT 100
#define USE_INSTANCE_ARRAY 0
glm::vec2 translations[100];

class Quads
{
public:
	void Init()
	{
		const float quad_vertices[] = {
			// vertex		color
			// positions     // colors
			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			0.05f,  0.05f,  0.0f, 1.0f, 1.0f
		};

		int index = 0;
		float offset = 0.1f;
		for (int y = -10; y < 10; y += 2)
		{
			for (int x = -10; x < 10; x += 2)
			{
				glm::vec2 translation;
				translation.x = (float)x / 10.0f + offset;
				translation.y = (float)y / 10.0f + offset;
				translations[index++] = translation;
			}
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 + 2 * sizeof(float)));
		glEnableVertexAttribArray(1);
#if USE_INSTANCE_ARRAY
		glGenBuffers(1, &m_Instance_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_Instance_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100,&translations, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(2, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif 
		glBindVertexArray(0);
	}
	void Draw()
	{
		glBindVertexArray(m_VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
	}
private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_Instance_VBO;

};




class Instancing_quads :public TestBase
{

	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");
		m_quadShader.loadFile(shaderPath + "10.1.Instancing_quads.vs", shaderPath + "10.1.Instancing_quads.ps");

		int index = 0;
		float offset = 0.1f;
		for (int y = -10; y < 10; y += 2)
		{
			for (int x = -10; x < 10; x += 2)
			{
				glm::vec2 translation;
				translation.x = (float)x / 10.0f + offset;
				translation.y = (float)y / 10.0f + offset;
				translations[index++] = translation;
			}
		};

		m_quadShader.use();
		for (LONGLONG i = 0; i < INSTANCE_COUNT; i++)
		{
			m_quadShader.setVec2("offsets[" + std::to_string(i) + "]", translations[i]);
		}
#if USE_INSTANCE_ARRAY
		m_quadShader.setBool("use_instance_array", true);
#else 
		m_quadShader.setBool("use_instance_array", false);
#endif 
	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_quads.Init();


	}
	virtual void Render()
	{
		//TestBase::Render();
		m_quadShader.use();
		m_quads.Draw();
	}
private:
	Shader m_quadShader;
	Quads m_quads;
};
START_TEST(Instancing_quads)
