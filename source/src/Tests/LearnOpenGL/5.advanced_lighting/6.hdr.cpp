//6.hdr.cpp
#include<TestOpenGLBase.h>
#include <RenderSystem/scene/QuadGeometry.h>
#include <RenderSystem/scene/Box.h>
#include <RenderSystem/utils/FBO.h>
#include <shader_m.h>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include "6.box.h"

using namespace RenderSystem;

std::vector<glm::vec3> lightPositions;
// colors
std::vector<glm::vec3> lightColors;
bool bHdr = true;
float exposure = 1.0f;
bool hdrKeyPressed = false;
class hdr :public TestBase
{
public:
	virtual void InitShader()
	{
		TestBase::InitShader();

		lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
		lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
		lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
		lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));

		lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
		lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
		lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
		lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

		std::string path = FileSystem::getPath("resources/Shaders/tests/LearnOpenGL/Shaders/5.advanced_lighting/");
		m_lightShader.loadFile(path + "6.lighting.vs", path + "6.lighting.ps");
		m_hdrShader.loadFile(path + "6.hdr.vs", path + "6.hdr.ps");
		// note that we're loading the texture as an SRGB texture
		m_tex = loadTexture(FileSystem::getPath("resources/textures/wood.png").c_str(), true); 

		m_lightShader.use();
		m_lightShader.setInt("diffuseTex", 0);

		// set light uniform
		for (int i = 0; i < lightPositions.size(); ++i)
		{
			m_lightShader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
			m_lightShader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
		}

		m_hdrShader.use();
		m_hdrShader.setInt("hdrBuffer", 0);

	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_cube.Init();
		m_quad.Init();
		m_fbo.CreateFbo();
		m_fbo.AddColorAttachment(GL_COLOR_ATTACHMENT0, SCR_WIDTH, SCR_HEIGHT, false, 0, GL_RGB16F, GL_RGBA);
		m_fbo.AddRBOAttachment(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);

		if (!m_fbo.CheckFboComplete())
		{
			std::cout << "check Complete fail" << std::endl;
			terminate();
		}
		m_fbo.Enable(false);
	}

	virtual void OnKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

		/*
		[in]	window	The window that received the event.
		[in]	key	The keyboard key that was pressed or released.
		[in]	scancode	The system-specific scancode of the key.
		[in]	action	GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
		[in]	mods	Bit field describing which modifier keys were held down.
		*/

		if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			exposure += 0.01;
			std::cout << "exposure:" << exposure << std::endl;

		}
		else if (key == GLFW_KEY_E && action == GLFW_PRESS)
		{
			exposure -= 0.01;
			std::cout << "exposure:" << exposure << std::endl;
		}
		else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			bHdr = !bHdr;
			std::cout <<(bHdr ? "hdr: on" : " hdr:off") << std::endl;
		}

	}
	virtual void Render()
	{
		//TestBase::Render();
		// 离屏渲染
		m_fbo.Enable(true);
		glClearColor(0.0f, 0.f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		m_lightShader.use();
	    glm::mat4 projection = glm::perspective(m_camera.Zoom, (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_camera.GetViewMatrix();
		m_lightShader.setMat4("projection", projection);
		m_lightShader.setMat4("view", view);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_tex);

		glm::mat4 model = glm::mat4();
		model = glm::translate(model,glm::vec3(0.0, 0.0, 25.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
		m_lightShader.setMat4("model", model);
		m_lightShader.setBool("inverse_normals", true);
		m_cube.Draw();
		m_fbo.Enable(false);
		
		// Render Quad
		glClearColor(0.0f, 0.f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		m_hdrShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_fbo.GetTexture(GL_COLOR_ATTACHMENT0));
		m_hdrShader.setBool("hdr", bHdr);
		m_hdrShader.setFloat("exposure", exposure);
		m_quad.Draw();

	}
private:
	Shader m_lightShader; // 接收光照的模型shader
	Shader m_hdrShader;   // FBO后处理shader
	Box2			 m_cube;
	QuadGeometry	 m_quad;
	unsigned int	 m_tex;
	Fbo			     m_fbo;

};
START_TEST(hdr)


/* 实现步骤：
1. 场景中添加隧道
2. 场景中添加灯光
3. 使用FBO渲染场景
4. 绘制Quad
*/