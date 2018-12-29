//2.1.stencil_tesing_wrap.cpp
#include<TestOpenGLBase.h>
#include <box.h>
#include <effect/OutLine.h>
#include <comm/model.h>

//using namespace RenderSystem;
class stencil_testing_wrap :public TestBase
{
	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");
		m_cubeShader.loadFile(shaderPath + "2.stencil_testing.vs", shaderPath + "2.stencil_testing.ps");
	

		m_cubeShader.use();
		m_cubeShader.setInt("texture1", 0);

		m_outLineEffect.Init(1.1, glm::vec3(1.0, 0.5, 0.5));

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

	virtual void InitRender()
	{
		/*TestBase::InitRender();*/
	/*	glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);*/

	}
	virtual void PreRender()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	virtual void Render()
	{
		TestBase::Render();

		// …Ë÷√cubeShader uniform≤Œ ˝
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
		m_outLineEffect.DrawOutLine(m_cube, m_cubeShader);
		
	}
private:
	RenderSystem::Box m_cube;
	RenderSystem::OutLine m_outLineEffect;
	Shader m_cubeShader;
	Shader m_groundShader;

	unsigned int m_cubeTexture;
};
START_TEST(stencil_testing_wrap)
