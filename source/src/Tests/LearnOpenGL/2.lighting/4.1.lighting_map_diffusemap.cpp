#include <TestOpenGLBase.h>
#include <box.h>
#include <shader_m.h>

using namespace RenderSystem;
class Material_Color :public TestBase
{
public:
	virtual void InitShader()
	{
		TestBase::InitShader();

		std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/2.lighting/");
		m_lightingShader.loadFile(shaderPath + "4.1.lighting_map_diffusemap.vs", shaderPath + "4.1.lighting_map_diffusemap.ps");
		m_lampShader.loadFile(shaderPath + "1.lamp.vs", shaderPath + "1.lamp.ps");
		m_lightingShader.use();
		m_lightingShader.setInt("material.diffuse", 0);
		m_lightingShader.setInt("material.specular", 1);
	}

	virtual void InitData()
	{
		TestBase::InitData();
		m_box.Init();
		m_lightBox.Init();

		std::string diffuseImage = FileSystem::getPath("resources/textures/container2.png");
		m_diffuseMap = loadTexture(diffuseImage.c_str());
		m_specularMap = loadTexture(FileSystem::getPath("resources/textures/container2_specular.png").c_str());

		m_camera.SetPose(glm::vec3(0, 0, 3));
	}

	virtual void PreRender()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	virtual void Render()
	{
		//TestBase::Render();
		// lighting
		glm::vec3 lightPos(.52f, 0.70f, 2.0f);

		m_lightingShader.use();
		m_lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		m_lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		m_lightingShader.setVec3("viewPos", m_camera.Position);


		m_lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		m_lightingShader.setFloat("material.shininess", 64.0f);

		m_lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		m_lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		m_lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		m_lightingShader.setVec3("light.position", lightPos);

		glm::mat4 projection = glm::perspective(glm::radians(m_camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = m_camera.GetViewMatrix();
		m_lightingShader.setMat4("projection", projection);
		m_lightingShader.setMat4("view", view);

		// world transformation
		glm::mat4 model;
		m_lightingShader.setMat4("model", model);

		glm::mat4 normalMat = glm::transpose(glm::inverse(model));
		m_lightingShader.setMat4("normalMat", normalMat);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_specularMap);

		m_box.Draw();

		
		// also draw the lamp object
		m_lampShader.use();
		m_lampShader.setMat4("projection", projection);
		m_lampShader.setMat4("view", view);
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		m_lampShader.setMat4("model", model);

		m_lightBox.Draw();

	}

private:
	Box m_box; // box 模型
	Box m_lightBox; // 用box模拟灯光
	Shader m_lightingShader; 
	Shader m_lampShader;
	unsigned int m_diffuseMap;
	unsigned int m_specularMap;
};

START_TEST(Material_Color)