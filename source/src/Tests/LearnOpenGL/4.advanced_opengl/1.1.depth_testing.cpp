//1.1.depth_testing.cpp
#include<TestOpenGLBase.h>
#include <box.h>

//using namespace RenderSystem;
class depth_testing :public TestBase
{

	virtual void InitRender()
	{
	
	}

	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");
		m_cubeShader.loadFile(shaderPath + "1.1.depth_testing.vs", shaderPath + "1.1.depth_testing.ps");
		m_cubeShader.use();
		m_cubeShader.setInt("texture1", 0);
	}
	virtual void InitData()
	{
		TestBase::InitData();
	
		unsigned int cubeTexture = loadTexture(FileSystem::getPath("resources/textures/marble.jpg").c_str());
		unsigned int floorTexture = loadTexture(FileSystem::getPath("resources/textures/metal.png").c_str());

		m_floorTexture = floorTexture;
		m_cubeTexture = cubeTexture;

		m_camera.SetPose(glm::vec3(0, 0, 3));
		m_cube.Init();

	}
	virtual void Render()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
		

		m_cubeShader.use();
		glm::mat4 model;
		glm::mat4 view = m_camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(m_camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		m_cubeShader.setMat4("view", view);
		m_cubeShader.setMat4("projection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		m_cubeShader.setMat4("model", model);
		m_cube.Draw();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		m_cubeShader.setMat4("model", model);
		m_cube.Draw();


		glBindTexture(GL_TEXTURE_2D, m_floorTexture);
		m_cubeShader.setMat4("model", glm::mat4());
		// floor

#if USE_RENDER_SYSTEM
		m_floor.Draw();
#else
		glBindVertexArray(m_floorVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
#endif 


	}
private:
	RenderSystem::Box m_cube;
	Shader m_cubeShader;
	Shader m_groundShader;
	unsigned int m_cubeTexture;
};
START_TEST(depth_testing)
