//3.1.blend_discard.cpp
#include<TestOpenGLBase.h>
#include <box.h>
#include <billboard.h>
using namespace RenderSystem;

static vector<glm::vec3> windows
{
	glm::vec3(-1.5f, 0.0f, -0.48f),
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
	glm::vec3(-0.3f, 0.0f, -2.3f),
	glm::vec3(0.5f, 0.0f, -0.6f)
};

class blend_discard :public TestBase
{
	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");
		m_cubeShader.loadFile(shaderPath + "1.1.depth_testing.vs", shaderPath + "1.1.depth_testing.ps");
		m_cubeShader.use();
		m_cubeShader.setInt("texture1", 0);

		m_billboardShader.loadFile(shaderPath + "3.2.blend_sort.vs", shaderPath + "3.2.blend_sort.ps");
		m_billboardShader.use();
		m_billboardShader.setInt("tex", 0);
	}
	virtual void InitData()
	{
		TestBase::InitData();

		unsigned int cubeTexture = loadTexture(FileSystem::getPath("resources/textures/marble.jpg").c_str());
		unsigned int floorTexture = loadTexture(FileSystem::getPath("resources/textures/metal.png").c_str());
		unsigned int transparentTexture = loadTexture(FileSystem::getPath("resources/textures/window.png").c_str());

		m_floorTexture = floorTexture;
		m_cubeTexture = cubeTexture;
		m_transparentTex = transparentTexture;

		m_camera.SetPose(glm::vec3(0, 0, 3));
		m_cube.Init();
		m_billboard.Init();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	virtual void PreRender()
	{
		TestBase::PreRender();
		// sort blend 
		m_sortedMap.clear();
		for (unsigned int i = 0; i < windows.size(); i++)
		{
			float distance = glm::length(m_camera.Position - windows[i]);
			m_sortedMap[distance] = windows[i];
		}
	}
	virtual void Render()
	{
		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_ALWAYS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))

		

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


		// draw billboard
		m_billboardShader.use();

		m_billboardShader.setMat4("projection", projection);
		m_billboardShader.setMat4("view", view);

		glBindTexture(GL_TEXTURE_2D, m_transparentTex);
		for (std::map<float, glm::vec3>::reverse_iterator it = m_sortedMap.rbegin(); it != m_sortedMap.rend(); ++it)
		{
			model = glm::mat4();
			model = glm::translate(model,it->second );
			m_billboardShader.setMat4("model", model);
			m_billboard.Draw();
		}


	}
private:
	RenderSystem::Box m_cube;
	Shader m_cubeShader;
	Shader m_groundShader;
	unsigned int m_cubeTexture;
	Billboard m_billboard;
	Shader m_billboardShader;

	unsigned int m_transparentTex;
	std::map<float, glm::vec3> m_sortedMap;
};

START_TEST(blend_discard)
