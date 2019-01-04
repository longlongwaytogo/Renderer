//4.normal_mapping.cpp
#include<TestOpenGLBase.h>
#include <NBillboard.h>
using namespace RenderSystem;

glm::vec3 lightPos(0.5f, 1.0f, 0.3f);

class normal_mapping :public TestBase
{
public:
	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string shaderPath = FileSystem::getPath("resources/Shaders/tests/LearnOpenGL/Shaders/5.advanced_lighting/");
		m_shader.loadFile(shaderPath + "4.normal_mapping.vs", shaderPath + "4.normal_mapping.ps");
		m_shader.use();
		m_shader.setInt("diffuseMap", 0);
		m_shader.setInt("normalMap", 1);
	}
	virtual void InitData()
	{
		m_diffuseMap = loadTexture(FileSystem::getPath("resources/textures/brickwall.jpg").c_str());
		m_normalMap = loadTexture(FileSystem::getPath("resources/textures/brickwall_normal.jpg").c_str());
		m_quad.Init();
		TestBase::InitData();
		m_camera.SetPose(glm::vec3(0, 0, 3));
	}
	virtual void Render()
	{

		glm::mat4 projection = glm::perspective(m_camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_camera.GetViewMatrix();
		
		m_shader.use();
		m_shader.setMat4("projection", projection);
		m_shader.setMat4("view", view);

		glm::mat4 model;
		model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show normal mapping from multiple directions
		m_shader.setMat4("model", model);
		m_shader.setVec3("viewPos", m_camera.Position);
		m_shader.setVec3("lightPos", lightPos);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_normalMap);

		m_quad.Draw();
		//TestBase::Render();
	}
private:
	unsigned int m_diffuseMap;
	unsigned int m_normalMap;
	Shader m_shader;
	NBillboard m_quad;
};
START_TEST(normal_mapping)
