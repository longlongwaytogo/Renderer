//////////////////////////////////////////////////////////////////////////
//@file: OutLine.h
//@brief: Use Stencil technique make the outline effect
//@author: szh
//@date: 2018/12/29
//////////////////////////////////////////////////////////////////////////
#ifndef RENDERER_INCLUDE_RENDERSYSTEM_OUTLINE_H
#define RENDERER_INCLUDE_RENDERSYSTEM_OUTLINE_H

#include <comm/shader_m.h>
#include <comm/model.h>
#include <glm/mat4x4.hpp>

namespace RenderSystem
{

	const std::string  vs = {
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoords;\n"

		"out vec2 TexCoords;\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"

		"void main()\n"
		"{\n"
		"TexCoords = aTexCoords;\n"
		"gl_Position = projection * view * model * vec4(aPos,1.0f);\n"
		"}\n"

	};

	const std::string ps = {
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec3 color;\n"

		"void main()\n"
		"{\n"
			"FragColor = vec4(color,1.0);\n"
		"}\n"

		};

	class OutLine
	{
	public:
		void Init(float lineSize, glm::vec3 color)
		{
			m_size = lineSize; // 1 +(lineSize / 10.0);
			m_color = color;

			m_outlineShader.loadFromSrc(vs, ps);
			m_outlineShader.use();
			m_outlineShader.setVec3("color", m_color);
			//glEnable(GL_STENCIL_TEST);
			//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			

		}

		void DrawOutLine(RenderSystem::Box& drawModel,Shader& modelShader)
		{
			

			modelShader.use();
			glm::mat4 model = glm::mat4();
			glm::mat4 view = glm::mat4();
			glm::mat4 projection = glm::mat4();

			modelShader.getMat4("model", model);
			modelShader.getMat4("view", view);
			modelShader.getMat4("projection", projection);

			// 1. ����ģ�建��д��
			glEnable(GL_STENCIL_TEST);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, 1, 0xFF); // ����Ƭ�ζ����Ը���ģ�滺��
			glStencilMask(0xFF); // ����ģ�滺��д��

			// 2. ����ģ��
			modelShader.use();
			drawModel.Draw();
			
			glDisable(GL_DEPTH_TEST);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			m_outlineShader.use();
			
			model = glm::scale(model, glm::vec3(m_size,m_size,m_size));
			m_outlineShader.setMat4("view", view);
			m_outlineShader.setMat4("projection", projection);
			m_outlineShader.setMat4("model", model);
			m_outlineShader.setVec3("color", m_color);
			drawModel.Draw();

			glDisable(GL_STENCIL_TEST);
			glStencilMask(0xFF); // �˴�ע�͵�����ģ�建���޷������
			glEnable(GL_DEPTH_TEST);

		}

	private:
		Shader m_outlineShader;
		float m_size;
		glm::vec3 m_color;
	};
}

/*
Stencil ʹ�ã�
1. glStencilFunc ����������ν���stencil����  (��������)

	��OpenGL�У�����ͨ������glStencilFunc()�������趨���Ƚ�������comparison function�����ο�ֵ��reference value���Լ��Ƚ������루comparison mask�������磬
	glStencilFunc(GL_EQUAL, // �Ƚ�����
	0x1, // �ο�ֵ
	0xff); // �Ƚ�������

2. glStenciOp ���ڴ���ģ����Խ����������θ���ģ�建�����ֵ  (���·���)
ģ�建�����ĸ�����ģ����ԵĽ���Լ���Ȳ��ԵĽ���������е���ϵ��ģ���������Ϊ��������������ֱ�ָ����Ӧ�ĸ��·�����
	1. ģ�����ʧ�ܡ�
	2. ģ�����ͨ��������Ȳ���ʧ�ܡ�
	3. ģ�����ͨ��������Ȳ���ͨ����
	����������е�һ������ʱ���ͻ�ִ��Ԥ���趨�ĸ��²�������OpenGL�У�����ʹ��glStencilOp()������Ϊ������������ֱ����ø��·��������磬
	glStencilOp(GL_KEEP, // ��һ��������·���
	GL_DECR, // �ڶ�������ĸ��·���
	GL_INCR); // ����������ĸ��·���


3. glStencilMask (glStenciOp�ĸ������������ڶԸ��½���������
	���ǿ���ͨ��д�����루write mask��������ģ��ֵָ������λ�ϵ���λ״̬��OpenGL�У��ṩ��glStencilMask()����������д�����롣
	���磬glStencilMask(0xff);
	http://www.cnblogs.com/aokman/archive/2010/12/13/1904723.html
*/
#endif // RENDERER_INCLUDE_RENDERSYSTEM_OUTLINE_H
