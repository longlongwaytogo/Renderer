//2.gamma_correction.cpp
#include<TestOpenGLBase.h>
using namespace RenderSystem;

glm::vec3 lightPositions[] = {
	glm::vec3(-3.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(3.0f, 0.0f, 0.0f)
};
glm::vec3 lightColors[] = {
	glm::vec3(0.25),
	glm::vec3(0.50),
	glm::vec3(0.75),
	glm::vec3(1.00)
};

class gamma_correction :public TestBase
{

public:
	virtual void InitShader()
	{
		std::string path = FileSystem::getPath("resources/Shaders/tests/LearnOpenGL/Shaders/5.advanced_lighting/");
		std::string vs = path + "2.gamma_correction.vs";
		std::string ps = path + "2.gamma_correction.ps";
		m_floorShader.loadFile(vs, ps);

		// load textures
		// -------------
		m_floorTexture = loadTexture(FileSystem::getPath("resources/textures/wood.png").c_str(),false);
		m_floorTextureGammaCorrected = loadTexture(FileSystem::getPath("resources/textures/wood.png").c_str(), true);
		glm::vec3 lightPos(0.0f, 1.0f, 0.0f);

		m_floorShader.use();
		m_floorShader.setInt("floorTexture", 0);
		m_floorShader.setBool("gamma", false);
		std::cout << "without gamma" << std::endl;

		
		glUniform3fv(glGetUniformLocation(m_floorShader.ID, "lightPositions"), 4, &lightPositions[0][0]);
		glUniform3fv(glGetUniformLocation(m_floorShader.ID, "lightColors"), 4, &lightColors[0][0]);
		m_gammaPressed = false;

	}

	virtual void OnKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			m_gammaPressed = !m_gammaPressed;
			if (m_gammaPressed)
				std::cout << "with gamma" << std::endl;
			else
				std::cout << "without gamma" << std::endl;

		}


	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_camera.SetPose(glm::vec3(0.0, 0.0, 3));
	}
	virtual void Render()
	{
		m_floorShader.use();
		glm::mat4 projection = glm::perspectiveFov(45.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.1f, 100.0f);
		m_floorShader.setMat4("projection", projection);
		glm::mat4 view = m_camera.GetViewMatrix();
		m_floorShader.setMat4("view", view);
		m_floorShader.setVec3("viewPos", m_camera.Position);
		m_floorShader.setBool("gamma", m_gammaPressed);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_gammaPressed? m_floorTextureGammaCorrected : m_floorTexture);
		m_floor.Draw();

		//TestBase::Render();
	}
private:
	bool m_gammaPressed;
	unsigned int m_floorTextureGammaCorrected;
};
START_TEST(gamma_correction)
