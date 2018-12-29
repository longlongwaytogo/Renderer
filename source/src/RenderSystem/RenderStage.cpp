#include <RenderSystem/RenderStage.h>
#include <comm/glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <comm/filesystem.h>

#include <RenderSystem/scene/SceneManager.h>
#include <RenderSystem/RenderPipeline.h>

RenderSystem::RenderStage::RenderStage(RenderWindowPtr window): m_window(window), m_bRun(false)
{

}

RenderSystem::RenderStage::~RenderStage()
{

}

bool RenderSystem::RenderStage::Init()
{
	LoadData();

	glm::mat4 mat;
	int n = mat.length();

	m_camera.SetPose(glm::vec3(0.0f, 0.0f, 5.0f));
	m_bRun = true;
	return true;
}

bool RenderSystem::RenderStage::LoadData()
{
	//// build and compile shaders
	std::string resoucePath = FileSystem::getPath("resources/shaders/3.model_loading/");

	m_defualtShader.loadFile((resoucePath + "1.model_loading.vs").c_str(), (resoucePath + "1.model_loading.fs").c_str());

	//// load models

	m_defualtModel.loadModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));

	// draw in wireFrame
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	SceneManager::GetInstance().CreateScene();
	// rend loop
	return true;
}

void RenderSystem::RenderStage::RenderLoop()
{
	
	void* ptr = m_window->GetNativeWindowHandle();
	GLFWwindow* window = static_cast<GLFWwindow*>(ptr);
	while (m_bRun && !glfwWindowShouldClose(window))
	{
		// pre-frame time logic
		float currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - m_lastFrame;

		m_lastFrame = currentFrame;

		// intput 

		UpdateEvent(m_deltaTime);

		UpdateData();

		// render
		glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// configure global opengl state
		glEnable(GL_DEPTH_TEST);

		DoRender();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
}

void RenderSystem::RenderStage::UpdateEvent(float deltaTime)
{
	m_window->UpdateEvent(deltaTime);
}

void RenderSystem::RenderStage::UpdateData()
{

}

void RenderSystem::RenderStage::DoRender()
{
	TestRender();
	//ourShader.use();

	//// view/ porjection transformations
	auto nodes = SceneManager::GetInstance().m_normalNodes;
	for (auto itr = nodes.begin(); itr != nodes.end(); ++itr)
	{
		//itr->second->GetModel()->Draw()
		RenderPipeline::GetInstance().DrawGeometryStage(NULL);
	}


}

void RenderSystem::RenderStage::Stop()
{
	m_bRun = false;
}

Camera& RenderSystem::RenderStage::GetCmaera()
{
	return m_camera;
}

void RenderSystem::RenderStage::TestRender()
{
	m_defualtShader.use();
	int w, h;
	m_window->GetWindowSize(w, h);
	glm::mat4 projection = glm::perspective(glm::radians(m_camera.Zoom),
		(float)w / (float)h, 0.1f, 100.0f);
	glm::mat4 view = m_camera.GetViewMatrix();
	m_defualtShader.setMat4("projection", projection);
	m_defualtShader.setMat4("view", view);

	//// render the loaded model
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	m_defualtShader.setMat4("model", model);
	m_defualtModel.Draw(m_defualtShader);
}


