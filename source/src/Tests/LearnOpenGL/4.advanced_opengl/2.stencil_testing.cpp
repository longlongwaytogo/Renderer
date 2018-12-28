//2.stencil_testing.cpp
#include<TestOpenGLBase.h>
#include <box.h>

//using namespace RenderSystem;
class stencil_testing :public TestBase
{

	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");
		m_cubeShader.loadFile(shaderPath + "2.stencil_testing.vs", shaderPath + "2.stencil_testing.ps");
		m_singleShader.loadFile(shaderPath + "2.stencil_testing.vs", shaderPath + "2.shaderSingleColor.ps");

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

	virtual void InitRender()
	{
		/*TestBase::InitRender();*/
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	}
	virtual void PreRender()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	}
	virtual void Render()
	{
//#if USE_RENDER_SYSTEM
//		m_floor.Draw();
//#else
//		glBindVertexArray(m_floorVAO);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//#endif 
		glStencilMask(0x00);
		TestBase::Render();
		// 1. �ڻ��ƣ���Ҫ��������ģ�����֮ǰ����ģ�庯������ΪGL_ALWAYS��ÿ�������Ƭ�α���Ⱦʱ����ģ�建�����Ϊ1��
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // ����Ƭ�ζ����Ը���ģ�滺��
		glStencilMask(0xFF); // ����ģ�滺��д��

		
		// ����singleShader uniform����
		m_singleShader.use();
		glm::mat4 model;
		glm::mat4 view = m_camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(m_camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		m_singleShader.setMat4("view", view);
		m_singleShader.setMat4("projection", projection);
		
		
		// ����cubeShader uniform����
		m_cubeShader.use();
		view = m_camera.GetViewMatrix();
		projection = glm::perspective(m_camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		m_cubeShader.setMat4("view", view);
		m_cubeShader.setMat4("projection", projection);
	
		// 2. ��Ⱦ����
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		m_cubeShader.setMat4("model", model);
		m_cube.Draw();
		
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		m_cubeShader.setMat4("model", model);
		m_cube.Draw();


		// 3 ����ģ��д���Լ���Ȳ��ԡ�
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // ������ģ�����ֵ�Ļ�ͨ������
		glStencilMask(0x00); // ����д�����룬0��ʾ��ֹд��
		glDisable(GL_DEPTH_TEST);

		// 4 ��ÿ����������һ��㡣
		m_singleShader.use();
		float scale = 1.1;
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		m_singleShader.setMat4("model", model);
		m_cube.Draw();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		m_singleShader.setMat4("model", model);
		m_cube.Draw();

		// 5 ʹ��һ����ͬ��Ƭ����ɫ�������һ�������ģ��߿���ɫ��
	    // 6 �ٴλ������壬��ֻ������Ƭ�ε�ģ��ֵ������1ʱ�Ż��ơ�
		 // 7 �ٴ�����ģ��д�����Ȳ��ԡ�
	
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);





	}
private:
	RenderSystem::Box m_cube;
	Shader m_cubeShader;
	Shader m_groundShader;
	Shader m_singleShader;
	unsigned int m_cubeTexture;
};
START_TEST(stencil_testing)
