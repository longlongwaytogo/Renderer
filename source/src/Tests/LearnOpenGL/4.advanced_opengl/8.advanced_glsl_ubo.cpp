//8.advanced_glsl_ubo.cpp
#include<TestOpenGLBase.h>
#include <box.h>
using namespace RenderSystem;
class advanced_glsl_ubo :public TestBase
{

	virtual void InitShader()
	{
		TestBase::InitShader();
		for (int i = 0; i < 4; i++)
		{
			unsigned int index = glGetUniformBlockIndex(m_shaders[0].ID, "Matrices");
			glUniformBlockBinding(m_shaders[i].ID, index, 0);
		}
	}
	virtual void InitData()
	{
		m_colors[0] = glm::vec4(1, 0, 0, 1);
		m_colors[1] = glm::vec4(0, 1, 0, 1);
		m_colors[2] = glm::vec4(0, 0, 1, 1);
		m_colors[3] = glm::vec4(1, 1, 0, 1);
	
		glm::mat4 model = glm::mat4();
		m_models[0] = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
		model = glm::mat4();
		m_models[1] = glm::translate(model, glm::vec3(-2.0f,2.0f, 0.0f));
		model = glm::mat4();
		m_models[2] = glm::translate(model, glm::vec3(2.0f, 2.0f, 0.0f));
		model = glm::mat4();
		m_models[3] = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));

		TestBase::InitData();
		m_box.Init();
		std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");
		for (LONGLONG i = 0; i < 4; i++)
			m_shaders[i].loadFile(shaderPath + "8.advanced_glsl_ubo.vs", shaderPath + "8.advanced_glsl_ubo.ps");
		
		// UBO
		glGenBuffers(1, &m_uboMatices);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatices);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboMatices, 0, 2 * sizeof(glm::mat4));
		
		// projection
		glm::mat4 projection = glm::perspectiveFov(45.0f, (float)SCR_WIDTH ,(float)SCR_HEIGHT, 0.1f, 100.0f);
		//glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
		m_camera.SetPose(glm::vec3(0, 0, 5));
	}
	virtual void Render()
	{
		TestBase::Render();

		// ubo update
		glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatices);
		glm::mat4 view = m_camera.GetViewMatrix();
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

		for (int i = 0; i < 4; i++)
		{
			m_shaders[i].use();
			m_shaders[i].setVec4("color", m_colors[i]);
			m_shaders[i].setMat4("model", m_models[i]);
			m_box.Draw();
		}
		
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
private:
	Box  m_box;
	Shader m_shaders[4];
	glm::vec4 m_colors[4];
	glm::mat4 m_models[4];
	unsigned int m_uboMatices;
};
START_TEST(advanced_glsl_ubo)
