//6.1.cubemap_skybox.cpp
#include<TestOpenGLBase.h>
#include <box.h>
#include <Skybox.h>

using namespace RenderSystem;
class cubemap_skybox :public TestBase
{

	virtual void InitShader()
	{
		TestBase::InitShader();

		//std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");
		std::string texturePath = FileSystem::getPath("resources/textures/skybox/");
		std::vector<std::string > faces
		{
			/*texturePath + "right.jpg",
			texturePath + "left.jpg",
			texturePath + "top.jpg",
			texturePath + "bottom.jpg",
			texturePath + "front.jpg",
			texturePath + "back.jpg"*/

			FileSystem::getPath("resources/textures/skybox/right.jpg"),
			FileSystem::getPath("resources/textures/skybox/left.jpg"),
			FileSystem::getPath("resources/textures/skybox/top.jpg"),
			FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
			FileSystem::getPath("resources/textures/skybox/back.jpg"),
			FileSystem::getPath("resources/textures/skybox/front.jpg")

		};


		m_skybox.loadCubemap(faces);

		std::string shaderPath = FileSystem::getPath("resources/shaders/comm/");
		m_skyboxShader.loadFile(shaderPath + "skybox.vs", shaderPath + "skybox.ps");
		m_skyboxShader.use();
		m_skyboxShader.setInt("skybox", 0);

		
	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_skybox.Init();
	}
	virtual void Render()
	{
		//TestBase::Render();
		glEnable(GL_DEPTH_TEST);
		m_skyboxShader.use();
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(1000, 1000, 1000));
		glm::mat4 view = m_camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(m_camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		m_skyboxShader.setMat4("model", model);
		m_skyboxShader.setMat4("view", view);
		m_skyboxShader.setMat4("projection", projection);

		m_skybox.Draw();
	}
private:

	Skybox m_skybox;
	Shader m_skyboxShader;
};
START_TEST(cubemap_skybox)
