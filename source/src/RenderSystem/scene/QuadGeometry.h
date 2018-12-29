//////////////////////////////////////////////////////////////////////////
//@file: QuadGeometry.h
//@brief: �����򵥵��ı��ζ�ά���ζ��󣬿��Ի��Ƶ���Ļ��
//@author: szh
//@date: 2018/12/26

#ifndef RENDERER_INCLUDE_RENDERSYSTEM_QUADGEOMETRY_H
#define RENDERER_INCLUDE_RENDERSYSTEM_QUADGEOMETRY_H

#include <comm/glad/glad.h>
namespace RenderSystem {
	class  QuadGeometry
	{
	public:

		// �����½�Ϊ�ο�ԭ��
		void Init(float left = 0, float down = 0, float right = 1, float top = 1)
		{
			// ��0--1 ��Χ�任�� -1--1 ��Χ���൱���ȷŴ�2����������ƽ��
			float l = 2 * left - 1; // -1
			float r = 2 * right - 1; // 1
			float t = 2 * top - 1;  // 1
			float d = 2 * down - 1;  // -1

			float quadVertics[] = {
				// positions        // texture Coords
				l, d, 0.0f, 0.0f, 0.0f,
				l, t, 0.0f, 0.0f, 1.0f,
				r, d, 0.0f, 1.0f, 0.0f,
				r, t, 0.0f, 1.0f, 1.0f,
			};
			

			glGenVertexArrays(1, &m_quadVAO);
			glBindVertexArray(m_quadVAO);

			glGenBuffers(1, &m_quadVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertics), &quadVertics, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

			glBindVertexArray(0);
		}

		void Draw()
		{
			glBindVertexArray(m_quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			//glDrawArrays(GL_QUADS, 0, 4); 
			glBindVertexArray(0);
		}

	private:
		int m_width;
		int m_height;

		unsigned int m_quadVAO;
		unsigned int m_quadVBO;
	};



}
#endif // RENDERER_INCLUDE_RENDERSYSTEM_QUADGEOMETRY_H