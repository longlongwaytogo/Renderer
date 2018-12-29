//////////////////////////////////////////////////////////////////////////
//@file Test OpenGL base 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <comm/filesystem.h>
#include <comm/shader_m.h>
#include <comm/camera.h>
#include <comm/model.h>

#define USE_RENDER_SYSTEM 1
#if USE_RENDER_SYSTEM
#include <plane.h>
using namespace RenderSystem;
#endif 
// utility function for loading a 2D texture from file
// ---------------------------------------------------

namespace Tests
{
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	float lastX = (float)SCR_WIDTH / 2.0;
	float lastY = (float)SCR_HEIGHT / 2.0;
	bool firstMouse = true;

	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	unsigned int loadTexture(char const * path);

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	/*
	Parameters
	[in]	window	The window that received the event.
	[in]	key	The keyboard key that was pressed or released.
	[in]	scancode	The system-specific scancode of the key.
	[in]	action	GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
	[in]	mods	Bit field describing which modifier keys were held down.
	*/
	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void processInput(GLFWwindow *window);

	class TestBase
	{
	public:
		TestBase()
		{
		}
		virtual int main(int argc, char** argv)
		{
			Init();
			while (!glfwWindowShouldClose(m_window))
			{
				float currentFrame = glfwGetTime();
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;

				OnProcessInput(m_window);
				PreRender();
				Render();
				glfwSwapBuffers(m_window);
				glfwPollEvents();
			}
			glfwTerminate();
			return 0;
		}

		void SetWindowName(const char* name) { m_windowName = name; }
		virtual void InitData()
		{
#if USE_RENDER_SYSTEM
			m_floor.Init();
#else

			float planeVertices[] = {
				// positions            // normals         // texcoords
				10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
				-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
				-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

				10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
				-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
				10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
			};
			// plane VAO
			unsigned int planeVAO, planeVBO;
			glGenVertexArrays(1, &planeVAO);
			glGenBuffers(1, &planeVBO);
			glBindVertexArray(planeVAO);
			glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindVertexArray(0);

			m_floorVAO = planeVAO;
			m_floorVBO = planeVBO;
#endif 
			
		}

		virtual void Init()
		{
			InitGLFW();
			InitShader();
			InitData();
			InitRender();
		}

		virtual void InitShader()
		{
			std::string path = FileSystem::getPath("resources/Shaders/tests/LearnOpenGL/Shaders/");
			std::string vs = path + "BaseGround.vs";
			std::string ps = path + "BaseGround.ps";
			m_floorShader.loadFile(vs, ps);

			// load textures
			// -------------
			m_floorTexture = loadTexture(FileSystem::getPath("resources/textures/wood.png").c_str());

			m_floorShader.use();
			m_floorShader.setInt("floorTexture", 0);

		}

		virtual void InitRender()
		{
			glEnable(GL_DEPTH_TEST);
		}

		virtual bool InitGLFW()
		{
			// glfw: initialize and configure
			// ------------------------------

			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// glfw window creation
		 
			if (m_windowName.empty())
			{
				m_windowName = "LearnOpenGL";
			}
			m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, m_windowName.c_str(), NULL, NULL);
			glfwMakeContextCurrent(m_window);
			if (m_window == NULL)
			{
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				return -1;
			}
			glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
			glfwSetCursorPosCallback(m_window, mouse_callback);
			glfwSetScrollCallback(m_window, scroll_callback);
			glfwSetKeyCallback(m_window, keyboard_callback);

			// tell GLFW to capture our mouse
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// glad: load all OpenGL function pointers
			// ---------------------------------------
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cout << "Failed to initialize GLAD" << std::endl;
				return -1;
			}

			//// configure global opengl state
			//// -----------------------------
			//glEnable(GL_DEPTH_TEST);
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		virtual void PreRender()
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		virtual void Render()
		{
				m_floorShader.use();
				glm::mat4 projection = glm::perspective(m_camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
				glm::mat4 view = m_camera.GetViewMatrix();
				m_floorShader.setMat4("projection", projection);
				m_floorShader.setMat4("view", view);
			
#if USE_RENDER_SYSTEM 
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_floorTexture);
				m_floor.Draw();
#else
				// floor
				glBindVertexArray(m_floorVAO);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_floorTexture);
				glDrawArrays(GL_TRIANGLES, 0, 6);
#endif 
		}

		virtual void OnFramebufferSize(GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
		virtual void OnMouse(GLFWwindow* window, double xpos, double ypos)
		{
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

			lastX = xpos;
			lastY = ypos;

			m_camera.ProcessMouseMovement(xoffset, yoffset);
		}
		virtual void OnScroll(GLFWwindow* window, double xoffset, double yoffset)
		{
			m_camera.ProcessMouseScroll(yoffset);
		}
		virtual void OnKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			/*
			[in]	window	The window that received the event.
			[in]	key	The keyboard key that was pressed or released.
			[in]	scancode	The system-specific scancode of the key.
			[in]	action	GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
			[in]	mods	Bit field describing which modifier keys were held down.
			*/

			
		}
		virtual void OnProcessInput(GLFWwindow *window)
		{
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			float cameraSpeed = 2.5 * deltaTime;
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				m_camera.ProcessKeyboard(FORWARD, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				m_camera.ProcessKeyboard(BACKWARD, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				m_camera.ProcessKeyboard(LEFT, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				m_camera.ProcessKeyboard(RIGHT, deltaTime);

		}
	protected:

#if USE_RENDER_SYSTEM
		Plane m_floor;
#else
		unsigned int m_floorVAO;
		unsigned int m_floorVBO;
#endif 
		GLFWwindow* m_window;
		Shader m_floorShader;
		unsigned int m_floorTexture;
		Camera m_camera;
		std::string m_windowName;
	public:
		static TestBase* s_TestInstance;

	};

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		TestBase::s_TestInstance->OnFramebufferSize(window, width, height);
	}
	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		TestBase::s_TestInstance->OnMouse(window, xpos, ypos);
	}
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		TestBase::s_TestInstance->OnScroll(window, xoffset, yoffset);
	}
	void processInput(GLFWwindow *window)
	{
		TestBase::s_TestInstance->OnProcessInput(window);
	}

	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		TestBase::s_TestInstance->OnKeyboard(window, key, scancode, action, mods);
	}
	unsigned int loadTexture(char const * path)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}
}

Tests::TestBase*  Tests::TestBase::s_TestInstance = nullptr;

#define START_TEST(className) \
void main(int argc, char** argv) \
{								 \
	Tests::TestBase::s_TestInstance = new className();			\
	Tests::TestBase::s_TestInstance->SetWindowName(#className); \
	Tests::TestBase::s_TestInstance->main(argc,argv);			\
	delete Tests::TestBase::s_TestInstance;						\
}
using namespace  Tests;

