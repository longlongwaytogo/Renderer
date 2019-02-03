//1.2.lighting_textured.cpp
#include<TestOpenGLBase.h>
#include <RenderSystem/scene/Sphere.h>

using namespace RenderSystem;

#if 0
// lights
const glm::vec3 lightPositions[] = {
	glm::vec3(-10.0f, 10.0f, 10.0f),
	glm::vec3(10.0f, 10.0f, 10.0f),
	glm::vec3(-10.0f, -10.0f, 10.0f),
	glm::vec3(10.0f, -10.0f, 10.0f),
};
const glm::vec3 lightColors[] = {
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f)
};
#else
// lights
glm::vec3 lightPositions[] = {
	glm::vec3(0.0, 0.0f, 10.0f),
};
glm::vec3 lightColors[] = {
	glm::vec3(150.0f, 150.0f, 150.0f)
};

#endif 
class lighting_textured :public TestBase
{

public:
	lighting_textured()
	{
		SCR_WIDTH = 1280, SCR_HEIGHT = 720;
	}
	virtual void InitShader()
	{
		TestBase::InitShader();
		// load textures
		/*
		�ڶԷ���������Ҳ������Щ��û��У������ʾ���Ͽ���������ȷ���������в���ʱ��
		����Щ�������ṩ�˹��ջ�����ɫ��Ϣ��������Ҫ�Ѳ������ʹ��һ��٤��ֵת�������Կռ��С�
		��Ҫ���Ѿ���������ɫ�ռ��е���������һЩHDR������������������͹����bump heights����
		����������������ɫ��Ϣ���������������Ĵ������ڷ�������������ʹ��sRGB�����ʽ��
		---------------------
		���ߣ�����˵Ů����Ҫ������ǿ
		��Դ��CSDN
		ԭ�ģ�https://blog.csdn.net/candycat1992/article/details/46228771
		��Ȩ����������Ϊ����ԭ�����£�ת���븽�ϲ������ӣ�
		*/
		m_albedoTex		= loadTexture(FileSystem::getPath("resources/textures/pbr/rusted_iron/albedo.png").c_str(),true); // �����Կռ���ɫ
		m_normalTex		= loadTexture(FileSystem::getPath("resources/textures/pbr/rusted_iron/normal.png").c_str(), false);
		m_metallicTex	= loadTexture(FileSystem::getPath("resources/textures/pbr/rusted_iron/metallic.png").c_str(), false);
		m_roughnessTex	= loadTexture(FileSystem::getPath("resources/textures/pbr/rusted_iron/roughness.png").c_str(), false);
		m_aoTex			= loadTexture(FileSystem::getPath("resources/textures/pbr/rusted_iron/ao.png").c_str(),true);

		std::string path = FileSystem::getPath("resources\\shaders\\tests\\LearnOpenGL\\Shaders\\6.pbr\\");

		m_bprShader.loadFile(path + "1.2.lighting_textured.vs", path + "1.2.lighting_textured.ps");
		m_bprShader.use();
		m_bprShader.setInt("albedoMap", 0);
		m_bprShader.setInt("normalMap", 1);
		m_bprShader.setInt("metallicMap", 2);
		m_bprShader.setInt("roughnessMap", 3);
		m_bprShader.setInt("aoMap", 4);

	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_sphere.Init();
		m_camera.SetPose(glm::vec3(0, 0, 3));
		
	}
	virtual void Render()
	{
		//TestBase::Render();

		m_bprShader.use();
		glm::mat4 projection = glm::perspective(m_camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		m_bprShader.setMat4("projection", projection);
		glm::mat4 view = m_camera.GetViewMatrix();
		m_bprShader.setMat4("view", view);
		m_bprShader.setVec3("camPos", m_camera.Position);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_albedoTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_normalTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_metallicTex);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_roughnessTex);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_aoTex);
		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5;

		// ����ģ��
		// render rows*column number of spheres with varying metallic/roughness
		// values scaled by rows and columns respectively
		glm::mat4 model = glm::mat4(0);
		for (int row = 0; row < nrRows; ++row)
		{
			for (int col = 0; col < nrColumns; ++col)
			{
				model = glm::mat4();
				model = glm::translate(model, glm::vec3(
					(float)(col - (nrColumns / 2)) * spacing,
					(float)(row - (nrRows / 2)) * spacing,
					0.0f));
				m_bprShader.setMat4("model", model);
				m_sphere.Draw();
			}
		}


		// render light source (simply re-render sphere at light positions)
		// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
		// keeps the codeprint small.
		// ���Ƶƹ�
		m_bprShader.use();
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];

			m_bprShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			m_bprShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			model = glm::mat4();
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			m_bprShader.setMat4("model", model);
			m_sphere.Draw();
		}
	}
private:
	Sphere m_sphere;
	Shader m_bprShader;
	GLuint m_albedoTex;
	GLuint m_normalTex;
	GLuint m_metallicTex;
	GLuint m_roughnessTex;
	GLuint m_aoTex;

};
START_TEST(lighting_textured)
