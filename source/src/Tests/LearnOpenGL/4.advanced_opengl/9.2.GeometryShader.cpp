//9.2.GeometryShader.cpp
#include<TestOpenGLBase.h>
#include <box.h>
using namespace RenderSystem;
class GeometryShader :public TestBase
{

	virtual void InitShader()
	{
		TestBase::InitShader();

		std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");
		m_explodeShader.loadFile((shaderPath + "9.2.GeometryShader.vs").c_str(), (shaderPath + "9.2.GeometryShader.ps").c_str(),
			(shaderPath + "9.2.GeometryShader_1.gs").c_str());
		m_explodeShader.use();
		m_explodeShader.setInt("diffuse", 0);


		m_modelShader.loadFile((shaderPath + "9.2.GeometryShader.vs").c_str(), (shaderPath + "9.2.GeometryShader_1.ps").c_str());
		m_modelShader.use();
		m_modelShader.setInt("diffuse", 0);

		m_normalDispalyShader.loadFile((shaderPath + "9.2.GeometryShader.vs").c_str(),
										(shaderPath + "9.2.GeometryShader_2.ps").c_str(),
										(shaderPath + "9.2.GeometryShader_2.gs").c_str());

	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_model.loadModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));

		m_camera.SetPose(glm::vec3(0, 7, 18));
	}
	virtual void Render()
	{
		TestBase::Render();

		m_explodeShader.use();
		glm::mat4 model = glm::mat4();
		m_explodeShader.setMat4("model", model);
		glm::mat4 view = m_camera.GetViewMatrix();
		m_explodeShader.setMat4("view", view);
		glm::mat4 projection = glm::perspectiveFov(45.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.1f, 100.0f);
		m_explodeShader.setMat4("projection", projection);
		m_explodeShader.setFloat("time", glfwGetTime());
		m_model.Draw(m_explodeShader);

		m_modelShader.use();
		model = glm::translate(model, glm::vec3(12, 0, 0));
		m_modelShader.setMat4("model", model);
		m_modelShader.setMat4("view", view);
		m_modelShader.setMat4("projection", projection);
		m_model.Draw(m_modelShader);

		m_normalDispalyShader.use();
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(12, 0, 0));
		m_normalDispalyShader.setMat4("model", model);
		m_normalDispalyShader.setMat4("view", view);
		m_normalDispalyShader.setMat4("projection", projection);
		m_model.Draw(m_normalDispalyShader);
	}
private:
	Shader m_explodeShader;
	Shader m_normalDispalyShader;
	Shader m_modelShader;
	Model m_model;
};
START_TEST(GeometryShader)
