//2.2.1.ibl_specular.cpp
#include<TestOpenGLBase.h>

#include <RenderSystem/scene/Sphere.h>
#include <RenderSystem/scene/Box.h>
#include <RenderSystem/scene/Box2.h>
#include <RenderSystem/scene/HDRCubeMap.h>

using namespace RenderSystem;

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

class ibl_specular :public TestBase
{
public:
	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string path = FileSystem::getPath("resources\\shaders\\tests\\LearnOpenGL\\Shaders\\6.pbr\\");
		m_bprShader.loadFile(path + "2.2.1.ibl_specular.vs", path + "2.2.1.ibl_specular.ps");
		m_backgroundShader.loadFile(path + "2.2.1.background.vs", path + "2.2.1.background.ps");

		m_hdrCubeMap.GenerateCubeMap(FileSystem::getPath("resources/textures/hdr/newport_loft.hdr"),true);
		m_hdrCubeMap.GenearteIrradianceCube();
		m_hdrCubeMap.GeneratePreFilterCubeMap();
		m_hdrCubeMap.GenerateBrdfLUT();

		m_bprShader.use();
		m_bprShader.setInt("irradianceMap", 0); // 传递irradianceCubeMap
		m_bprShader.setInt("prefilterMap", 1);
		m_bprShader.setInt("brdfLUT", 2);
		m_bprShader.setVec3("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
		m_bprShader.setFloat("ao", 1.0f);

		m_backgroundShader.use();
		m_backgroundShader.setInt("equirectangularMap", 0);

	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_sphere.Init();
		m_box.Init();

		m_camera.SetPose(glm::vec3(0, 0, 3));
	}

	virtual void PreRender()
	{
		TestBase::PreRender();
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glEnable(GL_DEPTH_TEST);
		// set depth function to less than AND equal for skybox depth trick.
		glDepthFunc(GL_LEQUAL);
		// enable seamless cubemap sampling for lower mip levels in the pre-filter map.
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
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


		// 将irradiance的CubeMap传递给bprshader中
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_hdrCubeMap.GetIrradianceCubeTex());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_hdrCubeMap.GetPreFilterCubeTex());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_hdrCubeMap.GetBrdfLUTTex());

		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5;

		// 绘制场景
		// render rows*column number of spheres with varying metallic/roughness
		// values scaled by rows and columns respectively
		glm::mat4 model = glm::mat4(0);
		for (int row = 0; row < nrRows; ++row)
		{
			m_bprShader.setFloat("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; ++col)
			{
				m_bprShader.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
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
		// 绘制灯光
		m_bprShader.use();
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			m_bprShader.setVec3("lightPositions[" + std::to_string(i) + "]", lightPositions[i]);
			m_bprShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			model = glm::mat4();
			model = glm::translate(model, lightPositions[i]);
			model = glm::scale(model, glm::vec3(0.5f));
			m_bprShader.setMat4("model", model);
			m_sphere.Draw();
		}

		//  render CubeMap
		// render skybox (render as last to prevent overdraw)
		m_backgroundShader.use();
		projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		m_backgroundShader.setMat4("projection", projection);
		m_backgroundShader.setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_hdrCubeMap.GetCubeMapTex());
		m_box.Draw();

		Shader& cubeMapShader = m_hdrCubeMap.GetCubeMapShader();
		cubeMapShader.use();
		cubeMapShader.setMat4("projection", projection);
		cubeMapShader.setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_hdrCubeMap.GetHDRMapTex());
		//glBindTexture(GL_TEXTURE_CUBE_MAP, m_hdrCubeMap.GetIrradianceCubeTex()); // display irradiance map
		//glBindTexture(GL_TEXTURE_CUBE_MAP, m_hdrCubeMap.GetPreFilterCubeTex()); // display irradiance map
		//m_box.Draw();

		// render brdfLUT quad
		m_hdrCubeMap.DrawBrdfLUTQuad();

	}
private:
	Sphere m_sphere;
	Box	   m_box;
	Shader m_bprShader;
	Shader m_cubeMapShader;
	Shader m_backgroundShader;
	HDRCubeMap m_hdrCubeMap;
};
START_TEST(ibl_specular)
