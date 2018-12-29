//5.1.framebuffers.cpp
#include<TestOpenGLBase.h>
#include <box.h>
#include <utils/FBO.h>
#include <scene/QuadGeometry.h>
#include <map>
#include <memory>

#define  POST_NUM 6
using namespace RenderSystem;
class framebuffers :public TestBase
{
	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");
		m_cubeShader.loadFile(shaderPath + "1.1.depth_testing.vs", shaderPath + "1.1.depth_testing.ps");
		m_cubeShader.use();
		m_cubeShader.setInt("texture1", 0);

		/*m_quadShader.loadFile(shaderPath + "5.1.framebuffers.vs", shaderPath + "5.1.framebuffers.ps");
		m_quadShader.use();
		m_quadShader.setInt("screenTexture", 0);*/

		m_postEffectMap[0] = std::make_shared<Shader>(shaderPath + "5.1.framebuffers.vs", shaderPath + "5.1.framebuffers.ps");
		m_postEffectMap[0]->use();
		m_postEffectMap[0]->setInt("screenTexture", 0);
		// post effect map invert
		m_postEffectMap[1] = std::make_shared<Shader>(shaderPath + "5.1.framebuffers.vs",shaderPath + "5.1.framebuffers_invert.ps");
		m_postEffectMap[1]->use();
		m_postEffectMap[1]->setInt("screenTexture", 0);

		// post effect map gray
		m_postEffectMap[2] = std::make_shared<Shader>(shaderPath + "5.1.framebuffers.vs", shaderPath + "5.1.framebuffers_gray.ps");
		m_postEffectMap[2]->use();
		m_postEffectMap[2]->setInt("screenTexture", 0);

		// post effect map sharpen
		m_postEffectMap[3] = std::make_shared<Shader>(shaderPath + "5.1.framebuffers.vs", shaderPath + "5.1.framebuffers_sharpen.ps");
		m_postEffectMap[3]->use();
		m_postEffectMap[3]->setInt("screenTexture", 0);

		// post effect map Edge-detection
		m_postEffectMap[4] = std::make_shared<Shader>(shaderPath + "5.1.framebuffers.vs", shaderPath + "5.1.framebuffers_edge_detection.ps");
		m_postEffectMap[4]->use();
		m_postEffectMap[4]->setInt("screenTexture", 0);

		// post effect map Blur
		m_postEffectMap[5] = std::make_shared<Shader>(shaderPath + "5.1.framebuffers.vs", shaderPath + "5.1.framebuffers_blur.ps");
		m_postEffectMap[5]->use();
		m_postEffectMap[5]->setInt("screenTexture", 0);

		 

	}

	virtual void InitData()
	{
		TestBase::InitData();

		unsigned int cubeTexture = loadTexture(FileSystem::getPath("resources/textures/container.jpg").c_str());
		unsigned int floorTexture = loadTexture(FileSystem::getPath("resources/textures/metal.png").c_str());

		m_floorTexture = floorTexture;
		m_cubeTexture = cubeTexture;

		m_camera.SetPose(glm::vec3(0, 0, 3));
		m_cube.Init();
		m_quad.Init();
		m_fbo.CreateDefualtFbo(SCR_WIDTH, SCR_HEIGHT);
		m_postEffectNum = 0;

	}

	virtual void OnKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS && key == GLFW_KEY_F2)
		{
			m_postEffectNum++;
			if (m_postEffectNum >= POST_NUM)
				m_postEffectNum = 0;
		}
	}
	virtual void InitRender()
	{
		
		TestBase::InitRender();
	}

	virtual void PreRender()
	{
		m_fbo.Enable();
		TestBase::PreRender();
		
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

		m_fbo.Enable(false);

		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
								  // clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//m_quadShader.use();
		m_postEffectMap[m_postEffectNum]->use();
		unsigned int tex = m_fbo.GetTexture(GL_COLOR_ATTACHMENT0);
		if (tex)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);
		}
		
		m_quad.Draw();
	}
	private:
		RenderSystem::Box m_cube;
		RenderSystem::QuadGeometry m_quad;
		Fbo m_fbo;
		Shader m_cubeShader;
		Shader m_groundShader;
		Shader m_quadShader; // »æÖÆËÄ±ßÐÎ
		unsigned int m_cubeTexture;

		unsigned int m_postEffectNum;

		std::map<unsigned int, std::shared_ptr<Shader>> m_postEffectMap;

};

START_TEST(framebuffers)
