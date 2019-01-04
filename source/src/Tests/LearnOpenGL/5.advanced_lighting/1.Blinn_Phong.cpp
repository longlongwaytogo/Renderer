//1.Blinn_Phong.cpp
#include<TestOpenGLBase.h>
using namespace RenderSystem;
class Blinn_Phong :public TestBase
{

public:
	virtual void InitShader()
	{
		std::string path = FileSystem::getPath("resources/Shaders/tests/LearnOpenGL/Shaders/5.advanced_lighting/");
		std::string vs = path + "1.Blinn_Phong.vs";
		std::string ps = path + "1.Blinn_Phong.ps";
		m_floorShader.loadFile(vs, ps);

		// load textures
		// -------------
		m_floorTexture = loadTexture(FileSystem::getPath("resources/textures/wood.png").c_str());

		glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

		m_floorShader.use();
		m_floorShader.setInt("floorTexture", 0);
		m_floorShader.setBool("blinn", false);
		m_floorShader.setVec3("lightPos", lightPos);
		m_blinnKeyPressed = false;
		std::cout << "Phong" << std::endl;

	}

	virtual void OnKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

		if (key == GLFW_KEY_B && action == GLFW_PRESS )
		{
			m_blinnKeyPressed = !m_blinnKeyPressed;
			if (m_blinnKeyPressed)
				std::cout << "Blinn-Phong" << std::endl;
			else
				std::cout << "Phong" << std::endl;
		}
		

	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_camera.SetPose(glm::vec3(0.0f, 0.0f, 3.0f));
	}
	virtual void Render()
	{
		m_floorShader.use();
		glm::mat4 projection = glm::perspectiveFov(45.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.1f, 100.0f);
		m_floorShader.setMat4("projection", projection);
		glm::mat4 view = m_camera.GetViewMatrix();
		m_floorShader.setMat4("view", view);
		m_floorShader.setVec3("viewPos", m_camera.Position);
		m_floorShader.setBool("blinn", m_blinnKeyPressed);
		TestBase::Render();
	}
private:
	bool m_blinnKeyPressed;
};
START_TEST(Blinn_Phong)
