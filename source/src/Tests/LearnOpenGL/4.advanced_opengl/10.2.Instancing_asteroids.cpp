//10.2.Instancing_asteroids.cpp
#include<TestOpenGLBase.h>
using namespace RenderSystem;
const 	GLuint amount = 100000;
class Instancing_asteroids :public TestBase
{

public:
	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");

		m_asteroidsShader.loadFile(shaderPath + "10.2.Instancing_asteroids.vs", shaderPath + "10.2.Instancing_asteroids.ps");
		m_planetShader.loadFile(shaderPath + "10.2.planet.vs", shaderPath + "10.2.planet.ps");
		
		m_planetShader.use();
		m_planetShader.setInt("tex", 0); 
		
		m_asteroidsShader.use();
		m_asteroidsShader.setInt("tex", 0);

		
	}
	virtual void InitData()
	{
		TestBase::InitData();

		m_asteroids.loadModel(FileSystem::getPath("resources/objects/rock/rock.obj"));
		m_planet.loadModel(FileSystem::getPath("resources/objects/planet/planet.obj"));

	
		glm::mat4* modelMatrices;
		modelMatrices = new glm::mat4[amount];
		srand(glfwGetTime()); // initialize random seed
		float radius = 150.0;
		float offset = 25.0f;
		for (unsigned int i = 0; i < amount; i++)
		{
			glm::mat4 model;
			// 1. translation: displace along circle with 'radius' in range [-offset, offset]
			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. scale: Scale between 0.05 and 0.25f
			float scale = (rand() % 20) / 100.0f + 0.05;
			model = glm::scale(model, glm::vec3(scale));

			// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. now add to list of matrices
			modelMatrices[i] = model;
		}

		unsigned int InstanceBuf;
		glGenBuffers(1, &InstanceBuf);
		glBindBuffer(GL_ARRAY_BUFFER, InstanceBuf);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

		// ÉèÖÃmodelmatrix
		for (unsigned int i = 0; i < m_asteroids.meshes.size(); i++)
		{
			unsigned int VAO = m_asteroids.meshes[i].VAO;
			glBindVertexArray(VAO);

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}


		delete[] modelMatrices;


		//m_asteroidsShader.use();
		//for (LONGLONG i = 0; i < amount; i++)
		//{
		//	//m_asteroidsShader.setMat4("")
		//}


		m_camera.SetPose(glm::vec3(0, 0, 155.0f));

	}
	virtual void Render()
	{
		//TestBase::Render();

		// draw planet
		m_planetShader.use();
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		m_planetShader.setMat4("model", model);
		glm::mat4 view = m_camera.GetViewMatrix();
		m_planetShader.setMat4("view", view);
		glm::mat4 projection = glm::perspectiveFov(45.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.1f, 1000.0f);
		m_planetShader.setMat4("projection", projection);

		m_planet.Draw(m_planetShader);

		m_asteroidsShader.use();
		m_asteroidsShader.setMat4("projection", projection);
		m_asteroidsShader.setMat4("view", view);
		
	/*	m_asteroids.Draw(m_asteroidsShader);*/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_asteroids.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
		for (unsigned int i = 0; i < m_asteroids.meshes.size(); i++)
		{
			glBindVertexArray(m_asteroids.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, m_asteroids.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}

	}
private:
	Model m_asteroids;
	Model m_planet;
	Shader m_planetShader;
	Shader m_asteroidsShader;

};
START_TEST(Instancing_asteroids)
