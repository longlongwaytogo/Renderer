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
			texturePath + "right.jpg",
			texturePath + "left.jpg",
			texturePath + "top.jpg",
			texturePath + "bottom.jpg",
			texturePath + "back.jpg",
			texturePath + "front.jpg",


			/*FileSystem::getPath("resources/textures/skybox/right.jpg"),
			FileSystem::getPath("resources/textures/skybox/left.jpg"),
			FileSystem::getPath("resources/textures/skybox/top.jpg"),
			FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
			FileSystem::getPath("resources/textures/skybox/back.jpg"),
			FileSystem::getPath("resources/textures/skybox/front.jpg")*/

		};


		m_skybox.loadCubemap(faces);

		std::string shaderPath = FileSystem::getPath("resources/shaders/comm/");
		m_skyboxShader.loadFile(shaderPath + "skybox.vs", shaderPath + "skybox.ps");
		m_skyboxShader.use();
		m_skyboxShader.setInt("skybox", 0);

		m_environmentShader.loadFile(shaderPath + "Environment.vs", shaderPath + "Environment.ps");
		m_environmentShader.use();
		m_environmentShader.setInt("skybox", 0);

		shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");
		m_modelShader.loadFile(shaderPath + "6.1.model.vs", shaderPath + "6.1.model.ps");
		m_modelShader.use();
		m_modelShader.setInt("skybox", 3);
	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_skybox.Init();
		m_box.Init();
		m_model.loadModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
		m_camera.SetPose(glm::vec3(0, 10, 10));
	}
	virtual void Render()
	{
		//TestBase::Render();
		glEnable(GL_DEPTH_TEST);
		m_skyboxShader.use();
		glm::mat4 model;
		//model = glm::scale(model, glm::vec3(1000, 1000, 1000));
		//glm::mat4 view = m_camera.GetViewMatrix();
		glm::mat4 view = glm::mat4(glm::mat3(m_camera.GetViewMatrix())); // 消除camera移动，让天空盒看起来无限大
		glm::mat4 projection = glm::perspective(m_camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		m_skyboxShader.setMat4("model", model);
		m_skyboxShader.setMat4("view", view);
		m_skyboxShader.setMat4("projection", projection);

		m_skybox.Draw();

		m_environmentShader.use();
		model = glm::mat4();
		view = m_camera.GetViewMatrix();
		m_environmentShader.setMat4("model", model);
		m_environmentShader.setMat4("view", view);
		m_environmentShader.setMat4("projection", projection);
		m_environmentShader.setVec3("cameraPos", m_camera.Position);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox.getTextureID());
		
		m_box.Draw();

		
		m_model.Draw(m_environmentShader);

		model = glm::translate(model, glm::vec3(8, 0.0, 0.0));
		m_modelShader.use(); 
		m_modelShader.setMat4("model", model);
		m_modelShader.setMat4("view", view);
		m_modelShader.setMat4("projection", projection);
		m_modelShader.setVec3("cameraPos", m_camera.Position);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox.getTextureID());
		
		m_model.Draw(m_modelShader);


	}
private:

	Skybox m_skybox;
	Shader m_skyboxShader;

	Box m_box;
	Shader m_environmentShader;

	Model m_model;

	Shader m_modelShader;
	 
};
START_TEST(cubemap_skybox)
