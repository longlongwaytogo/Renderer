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
		m_lightingShader.loadFile(shaderPath + "4.material.vs", shaderPath + "4.material.ps");
		m_lampShader.loadFile(shaderPath + "1.lamp.vs", shaderPath + "1.lamp.ps");

		
	}

	virtual void InitData()
	{
		TestBase::InitData();
		m_box.Init();
		m_lightBox.Init();

		std::string diffuseImage = FileSystem::getPath("resources/textures/")
		std::string diffuseImage = resourcePath + "";
		m_diffuseMap = loadTexture(diffuseImage);

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
		glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

		m_lightingShader.use();
		m_lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		m_lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		m_lightingShader.setVec3("viewPos", m_camera.Position);

		m_lightingShader.setInt("material.diffuse", 0);
		m_lightingShader.setVec3("material.specular", 0.5, 0.5, 0.5);
		m_lightingShader.setFloat("material.shininess", 32.0f);

		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

		m_lightingShader.setVec3("light.ambient", ambientColor);
		m_lightingShader.setVec3("light.diffuse", diffuseColor);
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
	Box m_box; // box ģ��
	Box m_lightBox; // ��boxģ��ƹ�
	Shader m_lightingShader; 
	Shader m_lampShader;
	unsigned int m_diffuseMap;
};

START_TEST(Material_Color)