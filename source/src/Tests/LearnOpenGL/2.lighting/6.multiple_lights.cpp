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
		m_lightingShader.loadFile(shaderPath + "6.multiple_lights.vs", shaderPath + "6.multiple_lights.ps");
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
		// positions all containers
		static glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		static glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
		};
		//TestBase::Render();
		// lighting
		glm::vec3 lightPos(.52f, 0.70f, 2.0f);

		m_lightingShader.use();
		// directional light 
		m_lightingShader.setVec3("dirLight.direction", -0.2, -1.0f, -0.3f);
		m_lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		m_lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		m_lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		//// point light 
		for (LONGLONG i = 0; i < 4; i++)
		{
			std::string index = std::to_string(i);
			std::string prefix = "pointLights[" + index + "]";
			m_lightingShader.setVec3("pointLights[" + index + "].position", pointLightPositions[i]);
			m_lightingShader.setVec3("pointLights[" + index + "].ambient", 0.05f, 0.05f, 0.05f);
			m_lightingShader.setVec3("pointLights[" + index + "].diffuse", 0.8f, 0.8f, 0.8f);
			m_lightingShader.setVec3("pointLights[" + index + "].specular", 1.0f, 1.0f, 1.0f);
			m_lightingShader.setFloat("pointLights[" + index + "].constant", 1.0f);
			m_lightingShader.setFloat("pointLights[" + index + "].linear", 0.09);
			m_lightingShader.setFloat("pointLights[" + index + "].quadratic", 0.032);
		}

		// spotLight
		m_lightingShader.setVec3("spotLight.position", m_camera.Position);
		m_lightingShader.setVec3("spotLight.direction", m_camera.Front);
		m_lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		m_lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		m_lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		m_lightingShader.setFloat("spotLight.constant", 1.0f);
		m_lightingShader.setFloat("spotLight.linear", 0.09);
		m_lightingShader.setFloat("spotLight.quadratic", 0.032);
		m_lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		m_lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		m_lightingShader.setFloat("material.shininess", 32.0f);
		m_lightingShader.setVec3("viewPos", m_camera.Position);
	
		glm::mat4 projection = glm::perspective(glm::radians(m_camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = m_camera.GetViewMatrix();
		m_lightingShader.setMat4("projection", projection);
		m_lightingShader.setMat4("view", view);
 

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_specularMap);

		//render cube 
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			m_lightingShader.setMat4("model", model);

			glm::mat4 normalMat = glm::transpose(glm::inverse(model));
			m_lightingShader.setMat4("normalMat", normalMat);

			m_box.Draw();
		}
		
		// also draw the lamp object
		m_lampShader.use();
		m_lampShader.setMat4("projection", projection);
		m_lampShader.setMat4("view", view);
		

		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
			m_lampShader.setMat4("model", model);

			m_lightBox.Draw();
		}
		

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