//2.2.2.ibl_specular_textured.cpp
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

struct MaterialMap
{
	void set(unsigned int albedo,unsigned int normal, unsigned int metallic, unsigned int roughness, unsigned int ao)	
	{
		albedoMap = albedo;
		normalMap = normal;
		metallicMap = metallic;
		roughnessMap = roughness;
		aoMap = ao;
	}
	unsigned int albedoMap;
	unsigned int normalMap;
	unsigned int metallicMap;
	unsigned int roughnessMap;
	unsigned int aoMap;
};
class ibl_specular_textured :public TestBase
{
public:
	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string path = FileSystem::getPath("resources\\shaders\\tests\\LearnOpenGL\\Shaders\\6.pbr\\");
		m_bprShader.loadFile(path + "2.2.2.ibl_specular_textured.vs", path + "2.2.2.ibl_specular_textured.ps");
		m_backgroundShader.loadFile(path + "2.2.1.background.vs", path + "2.2.1.background.ps");

		m_hdrCubeMap.GenerateCubeMap(FileSystem::getPath("resources/textures/hdr/newport_loft.hdr"), true);
		m_hdrCubeMap.GenearteIrradianceCube();
		m_hdrCubeMap.GeneratePreFilterCubeMap();
		m_hdrCubeMap.GenerateBrdfLUT();

		m_bprShader.use();
		m_bprShader.setInt("irradianceMap", 0); // 传递irradianceCubeMap
		m_bprShader.setInt("prefilterMap", 1);
		m_bprShader.setInt("brdfLUT", 2);
		m_bprShader.setInt("albedoMap",3);
		m_bprShader.setInt("normalMap", 4);
		m_bprShader.setInt("metallicMap", 5);
		m_bprShader.setInt("roughnessMap", 6);
		m_bprShader.setInt("aoMap", 7);

		m_backgroundShader.use();
		m_backgroundShader.setInt("equirectangularMap", 0);

	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_sphere.Init();
		m_box.Init();

		bool bGamma = true;
		// load texture
		std::string path = FileSystem::getPath("resources/textures/pbr/");

		m_ironMaterial.set(loadTexture(path + "rusted_iron/albedo.png", bGamma),
			loadTexture(path + "rusted_iron/normal.png"),
			loadTexture(path + "rusted_iron/metallic.png"),
			loadTexture(path + "rusted_iron/roughness.png"),
			loadTexture(path + "rusted_iron/ao.png"));
		m_goldMaterial.set(loadTexture(path + "gold/albedo.png", bGamma),
			loadTexture(path + "gold/normal.png"),
			loadTexture(path + "gold/metallic.png"),
			loadTexture(path + "gold/roughness.png"),
			loadTexture(path + "gold/ao.png"));
		m_grassMaterial.set(loadTexture(path + "grass/albedo.png", bGamma),
			loadTexture(path + "grass/normal.png"),
			loadTexture(path + "grass/metallic.png"),
			loadTexture(path + "grass/roughness.png"),
			loadTexture(path + "grass/ao.png"));
		m_plasticMaterial.set(loadTexture(path + "plastic/albedo.png", bGamma),
			loadTexture(path + "plastic/normal.png"),
			loadTexture(path + "plastic/metallic.png"),
			loadTexture(path + "plastic/roughness.png"),
			loadTexture(path + "plastic/ao.png"));
		m_wallMaterial.set(loadTexture(path + "wall/albedo.png", bGamma),
			loadTexture(path + "wall/normal.png"),
			loadTexture(path + "wall/metallic.png"),
			loadTexture(path + "wall/roughness.png"),
			loadTexture(path + "wall/ao.png"));

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


		// 绘制场景
		// render rows*column number of spheres with varying metallic/roughness
		// values scaled by rows and columns respectively
		glm::mat4 model = glm::mat4(0);


		// 将irradiance的CubeMap传递给bprshader中
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_hdrCubeMap.GetIrradianceCubeTex());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_hdrCubeMap.GetPreFilterCubeTex());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_hdrCubeMap.GetBrdfLUTTex());
		
		// rusted iron
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_ironMaterial.albedoMap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_ironMaterial.normalMap);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_ironMaterial.metallicMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, m_ironMaterial.roughnessMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, m_ironMaterial.aoMap);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));
		m_bprShader.setMat4("model", model);
		m_sphere.Draw();

		// gold
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_goldMaterial.albedoMap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_goldMaterial.normalMap);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_goldMaterial.metallicMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, m_goldMaterial.roughnessMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, m_goldMaterial.aoMap);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-3.0, 0.0, 2.0));
		m_bprShader.setMat4("model", model);
		m_sphere.Draw();
	 
		// grass
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_grassMaterial.albedoMap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_grassMaterial.normalMap);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_grassMaterial.metallicMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, m_grassMaterial.roughnessMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, m_grassMaterial.aoMap);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0, 0.0, 2.0));
		m_bprShader.setMat4("model", model);
		m_sphere.Draw();

		// plastic
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_plasticMaterial.albedoMap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_plasticMaterial.normalMap);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_plasticMaterial.metallicMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, m_plasticMaterial.roughnessMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, m_plasticMaterial.aoMap);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.0, 0.0, 2.0));
		m_bprShader.setMat4("model", model);
		m_sphere.Draw();

		// wall
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_wallMaterial.albedoMap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_wallMaterial.normalMap);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_wallMaterial.metallicMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, m_wallMaterial.roughnessMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, m_wallMaterial.aoMap);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(3.0, 0.0, 2.0));
		m_bprShader.setMat4("model", model);
		m_sphere.Draw();

	

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
		// m_hdrCubeMap.DrawBrdfLUTQuad();

	}
private:
	Sphere m_sphere;
	Box	   m_box;
	Shader m_bprShader;
	Shader m_cubeMapShader;
	Shader m_backgroundShader;
	HDRCubeMap m_hdrCubeMap;
	
	MaterialMap m_ironMaterial;
	MaterialMap m_goldMaterial;
	MaterialMap m_grassMaterial;
	MaterialMap m_plasticMaterial;
	MaterialMap m_wallMaterial;
	
};
START_TEST(ibl_specular_textured)
