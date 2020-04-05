//3.1.1.shadow_mapping_depth.cpp
#include<TestOpenGLBase.h>
#include <RenderSystem/scene/Box2.h>
#include <RenderSystem/utils/FBO.h>
#include <RenderSystem/scene/QuadGeometry.h>
//#include <glm/matrix.hpp>
//#include <glm/mat4x4.hpp>
//#include <glm/vec3.hpp>
#include <glm/glm.hpp>

using namespace RenderSystem;
const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;

class shadow_mapping_depth :public TestBase
{

public:
	virtual void InitShader()
	{
		TestBase::InitShader();

		std::string path = FileSystem::getPath("resources/Shaders/tests/LearnOpenGL/Shaders/5.advanced_lighting/");
		std::string vs = path + "3.1.1.shadow_mapping_depth.vs";
		std::string ps = path + "3.1.1.shadow_mapping_depth.ps";
		m_shadowShader.loadFile(vs, ps);

		std::string debugvs = path + "3.1.1.debug.vs";
		std::string debugps = path + "3.1.1.debug.ps";
		m_debugShader.loadFile(debugvs, debugps);

		std::string scenevs = path + "3.1.1.shadow_mapping.vs";
		std::string sceneps = path + "3.1.1.shadow_mapping.ps";
		m_sceneShader.loadFile(scenevs, sceneps);

		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		unsigned int depthMapFBO;
		glGenFramebuffers(1, &depthMapFBO);
		m_fboId = depthMapFBO;
		// create depth texture
		 
		glGenTextures(1, &m_depthMap);
		glBindTexture(GL_TEXTURE_2D, m_depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//m_fbo.CreateFbo();
		{
			// texture
		/*	glGenTextures(1,&m_depthMap);
			glBindTexture(GL_TEXTURE_2D, m_depthMap);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
		//	m_fbo.SetColorAttachment(GL_DEPTH_COMPONENT,m_depthMap);
			//m_fbo.AddRBOAttachment(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
		}
		/*	if (!m_fbo.CheckFboComplete())
			{
				std::cout << "check Complete fail" << std::endl;
				terminate();
			}
	*/
		//m_fbo.Enable(false);

	/*	if (!m_fbo.CheckFboComplete())
		{
			std::cout << "check Complete fail" << std::endl;
			terminate();
		}*/
		//m_fbo.Enable(false);

		

	}
	virtual void InitData()
	{
		m_box.Init();
		m_debugQuad.Init(0, 0, 0.2, 0.2);
		TestBase::InitData();
		m_camera.SetPose(glm::vec3(0, 0, 5));
	}

	void renderScene(Shader& shader)
	{
		glm::mat4 projection = glm::perspective(m_camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_camera.GetViewMatrix();
		glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

		/*float near_plane = 0.1;
		float far_plane = 100.0;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		*/
		shader.use();

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		/*shader.setMat4("projection", lightProjection);
		shader.setMat4("view", lightView);*/
	 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_floorTexture);
		m_floor.Draw();

		glm::mat4 model;


		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shader.setMat4("model", model);
		m_box.Draw();// render box

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shader.setMat4("model", model);
		m_box.Draw();// render box
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25));
		shader.setMat4("model", model);
		m_box.Draw();// render box

		// render ground
		//TestBase::Render();
	
	}
	virtual void Render()
	{
		 // Render Shadow Map 
	 
		// render fbo
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);*/
		renderScene(m_shadowShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// render scene 
		m_sceneShader.setInt("diffuseMap", 0);
		m_sceneShader.setInt("shadowMap", 1);
		renderScene(m_sceneShader);

		// debug
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_depthMap);
		m_debugShader.use();
		m_debugShader.setInt("diffuseMap",0);
		m_debugQuad.Draw();
	 

	   // render quad 

	}
private:
	Shader m_shadowShader; // 绘制shadow贴图shader
	Shader m_sceneShader;    // 绘制场景使用的shader
	Shader m_debugShader;  // 
	Box2 m_box; // 
	Fbo m_fbo;
	unsigned int m_fboId;
	unsigned int m_depthMap;
	QuadGeometry	 m_debugQuad;
};
START_TEST(shadow_mapping_depth)
