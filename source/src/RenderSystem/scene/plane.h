//////////////////////////////////////////////////////////////////////////
//@file box.h
//@brief box Ä£ÐÍ
//@author: szh
//@date: 2018/12/27

#ifndef RENDERER_INCLUDE_RENDERSYSTEM_PLANE_H
#define RENDERER_INCLUDE_RENDERSYSTEM_PLANE_H
#include <comm/glad/glad.h>

namespace RenderSystem {
	class Plane
	{
	public:
		void Init()
		{
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

			m_VAO = planeVAO;
			m_VBO = planeVBO;
			
		}

		void Draw()
		{	
				glBindVertexArray(m_VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
		}

	public:
		unsigned int m_VAO;
		unsigned int m_VBO;
	};
}

#endif // RENDERER_INCLUDE_RENDERSYSTEM_PLANE_H