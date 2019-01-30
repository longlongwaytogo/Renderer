//7.bloom.cpp
#include<TestOpenGLBase.h>
#include <RenderSystem/utils/FBO.h>
#include <RenderSystem/scene/Box.h>
#include <RenderSystem/scene/QuadGeometry.h>
#include "6.box.h"
#include <shader_m.h>
#include <filesystem.h>

using namespace RenderSystem;

/*
*Bloom 效果实现步骤
	1. 创建hdrFbo，包含有连个颜色缓冲区，进行多目标渲染， 
	2. 绘制hdr，并分离高光[绘制模型包括普通场景和发光物体，发光物体需要使用特殊shader绘制，用于提取大于1.0的亮度信息]
	4. 对高光成分进行模糊处理
	5. 混合hdr效果和模糊结果，生成bloom效果
*/
class Bloom
{
public:
	Bloom()
	{

	}

	~Bloom()
	{

	}
	void Init()
	{
		m_hdrFbo.CreateFbo();
		m_hdrFbo.AddColorAttachment(GL_COLOR_ATTACHMENT0, SCR_WIDTH, SCR_HEIGHT, false, 4, GL_RGBA16, GL_RGBA); // 绘制普通场景
		m_hdrFbo.AddColorAttachment(GL_COLOR_ATTACHMENT1, SCR_WIDTH, SCR_HEIGHT, false, 4, GL_RGBA16, GL_RGBA); // 提取亮度大于1.0的发光物体
		m_hdrFbo.AddRBOAttachment(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
		if (!m_hdrFbo.CheckFboComplete())
		{
			std::cout << "check Complete fail" << std::endl;
			terminate();
		}
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);

		m_hdrFbo.Enable(false);
	}

	void StartBloom()
	{
		m_hdrFbo.Enable(true);
	}

	void EndBloom()
	{
		m_hdrFbo.Enable(false);
	}
	void Finish()
	{

	}


	Fbo& GetFbo() { return m_hdrFbo; }
 

private:
	
	Fbo m_hdrFbo; 
};

std::vector<glm::vec3> lightPositions;
// colors
std::vector<glm::vec3> lightColors;
bool bBloom = true;
float exposure = 1.0f;

class bloom :public TestBase
{

public:
	virtual void InitShader()
	{
		TestBase::InitShader();

		m_containerTexture = loadTexture(FileSystem::getPath("resources/textures/container2.png").c_str(), true);
		m_woodTexture = loadTexture(FileSystem::getPath("resources/textures/wood.png").c_str(), true);
		std::string shaderPath = FileSystem::getPath("resources/Shaders/tests/LearnOpenGL/Shaders/5.advanced_lighting//");
		m_boxShader.loadFile(shaderPath + "7.bloom.vs", shaderPath + "7.bloom.ps");
		m_lightShader.loadFile(shaderPath + "7.bloom.vs", shaderPath + "7.light.ps");
		m_blurShader.loadFile(shaderPath + "7.blur.vs", shaderPath + "7.blur.ps");
		m_bloomFinal.loadFile(shaderPath + "7.bloomFinal.vs", shaderPath + "7.bloomFinal.ps");

		m_boxShader.use();
		m_boxShader.setInt("diffuseTexture", 0);
		m_blurShader.use();
		m_blurShader.setInt("tex", 0);
		m_bloomFinal.use();
		m_bloomFinal.setInt("hdrScene", 0);
		m_bloomFinal.setInt("bloomBlur", 1);

		m_bloom.Init();

		lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
		lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
		lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
		lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));

		lightColors.push_back(glm::vec3(2.0f, 2.0f, 2.0f));
		lightColors.push_back(glm::vec3(1.5f, 0.0f, 0.0f));
		lightColors.push_back(glm::vec3(0.0f, 0.0f, 1.5f));
		lightColors.push_back(glm::vec3(0.0f, 1.5f, 0.0f));

		m_boxShader.use();
		for (unsigned int i = 0; i < lightPositions.size(); i++)
		{
			m_boxShader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
			m_boxShader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
		}

	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_box.Init();
		m_Quad.Init();
		for (int i = 0; i < 2; ++i)
		{
			m_pingpongFbo[i].CreateFbo();
			m_pingpongFbo[i].AddColorAttachment(GL_COLOR_ATTACHMENT0, SCR_WIDTH, SCR_HEIGHT, false, 4, GL_RGBA16, GL_RGBA);
			m_pingpongFbo[i].AddRBOAttachment(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);

			if (!m_pingpongFbo[i].CheckFboComplete())
			{
				std::cout << "create fbo " << i <<" fail" << std::endl;
				terminate();
			}
			m_pingpongFbo[i].Enable(false);
		}
		
		m_camera.SetPose(glm::vec3(0.0f, 0.0f, 5.0f));

	}
	virtual void Render()
	{
		m_bloom.StartBloom();

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 绘制box
		m_boxShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_woodTexture);
		// 绘制默认地面
		//TestBase::Render();
		glm::mat4 projection = glm::perspective(m_camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_camera.GetViewMatrix();
		
		m_boxShader.setMat4("projection", projection);
		m_boxShader.setMat4("view", view);

		for (unsigned int i = 0; i < lightPositions.size(); i++)
		{
			m_boxShader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
			m_boxShader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
		}

		glm::mat4 model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
		model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
		m_boxShader.setMat4("model", model);
		m_box.Draw();

		glBindTexture(GL_TEXTURE_2D, m_containerTexture);
	
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		m_boxShader.setMat4("model", model);
		m_box.Draw();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		m_boxShader.setMat4("model", model);
		m_box.Draw();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		m_boxShader.setMat4("model", model);
		m_box.Draw();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
		model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(1.25));
		m_boxShader.setMat4("model", model);
		m_box.Draw();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
		model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		m_boxShader.setMat4("model", model);
		m_box.Draw();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		m_boxShader.setMat4("model", model);
		m_box.Draw();

		// 绘制发光物体
		m_lightShader.use();
		m_lightShader.setMat4("projection", projection);
		m_lightShader.setMat4("view", view);
		for (unsigned int i = 0; i < lightPositions.size(); i++)
		{
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(lightPositions[i]));
			model = glm::scale(model, glm::vec3(0.25f));
			m_lightShader.setMat4("model", model);
			m_lightShader.setVec3("lightColor", lightColors[i]);
			m_box.Draw();
		}
	
		m_bloom.EndBloom();

		//// 高斯模糊处理
		bool horizontal = true; 
		bool first_iteration = true;
		unsigned int amount = 10;

		
		for (unsigned int i = 0; i < amount; i++)
		{
			int index = horizontal;
			m_pingpongFbo[index].Enable(true);
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_blurShader.use();
			m_blurShader.setBool("horizontal", horizontal);
			// 第一次从bloom的fbo中获取GL_COLOR_ATTACHMENT1，它内部保存了高光信息,
			// 之后从m_pingpongFbo中获取渲染后的结果
			glBindTexture(GL_TEXTURE_2D, first_iteration ? m_bloom.GetFbo().\
				GetTexture(GL_COLOR_ATTACHMENT1) : m_pingpongFbo[!horizontal].\
				GetTexture(GL_COLOR_ATTACHMENT0));

			m_Quad.Draw();

			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
			m_pingpongFbo[index].Enable(false);
		}


		// 3. now render floating point color buffer to 2D quad and tonemap HDR colors
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_bloom.GetFbo().GetTexture(GL_COLOR_ATTACHMENT0));
		glActiveTexture(GL_TEXTURE1);
		int index = !horizontal;
		glBindTexture(GL_TEXTURE_2D, m_pingpongFbo[index].GetTexture(GL_COLOR_ATTACHMENT0));
		m_bloomFinal.use();
		m_bloomFinal.setInt("bloom", bBloom);
		m_bloomFinal.setFloat("exposure", exposure);
		m_Quad.Draw();
	}

	virtual void OnKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			
		}
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			
		}

		if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			if (exposure > 0.0f)
				exposure -= 0.001f;
			else
				exposure = 0.0f;

			std::cout << "exposure:" << exposure << std::endl;
		}
		else if (key == GLFW_KEY_E && action == GLFW_PRESS)
		{
			exposure += 0.001f;
			std::cout << "exposure:" << exposure << std::endl;

		}
		else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			bBloom = !bBloom;
			std::cout << " bloom:" << (bBloom ? "true" : "false") << std::endl;
		}
	}
private:
	Bloom			m_bloom;		 // 
	Box2			m_box;			 //　场景
	QuadGeometry	m_Quad;		// 最终生成图形
 
	Shader	m_boxShader;	 // 绘制box的shader
	Shader	m_lightShader;	 // 绘制发光物体Shader
	Shader  m_blurShader;	 // 高斯模糊处理Shader
	Shader  m_bloomFinal;	 // 最终合成效果shader
	Fbo		m_pingpongFbo[2];   // 用于模糊处理的fbo对象
	GLuint	m_containerTexture;
	GLuint  m_woodTexture;
};
START_TEST(bloom)
