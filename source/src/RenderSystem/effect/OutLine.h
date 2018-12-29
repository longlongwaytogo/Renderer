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

			// 1. 启用模板缓存写入
			glEnable(GL_STENCIL_TEST);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有片段都可以更新模版缓冲
			glStencilMask(0xFF); // 启用模版缓冲写入

			// 2. 绘制模型
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
			glStencilMask(0xFF); // 此处注释掉，则模板缓冲无法被清空
			glEnable(GL_DEPTH_TEST);

		}

	private:
		Shader m_outlineShader;
		float m_size;
		glm::vec3 m_color;
	};
}

/*
Stencil 使用：
1. glStencilFunc 用来设置如何进行stencil测试  (测试条件)

	在OpenGL中，可以通过调用glStencilFunc()函数来设定，比较条件（comparison function）、参考值（reference value）以及比较用掩码（comparison mask）。例如，
	glStencilFunc(GL_EQUAL, // 比较条件
	0x1, // 参考值
	0xff); // 比较用掩码

2. glStenciOp 用于处理模板测试结果，即，如何更新模板缓冲的数值  (更新方法)
模板缓冲区的更新与模板测试的结果以及深度测试的结果有着密切的联系。模板操作可以为下述三种情况，分别指定相应的更新方法。
	1. 模板测试失败。
	2. 模板测试通过，但深度测试失败。
	3. 模板测试通过，且深度测试通过。
	当上述情况中的一个发生时，就会执行预先设定的更新操作。在OpenGL中，可以使用glStencilOp()函数来为上述三种情况分别设置更新方法。例如，
	glStencilOp(GL_KEEP, // 第一种情况更新方法
	GL_DECR, // 第二种情况的更新方法
	GL_INCR); // 第三种情况的更新方法


3. glStencilMask (glStenciOp的辅助函数，用于对更新进行修正）
	我们可以通过写入掩码（write mask）来更新模板值指定比特位上的置位状态。OpenGL中，提供了glStencilMask()函数来设置写入掩码。
	例如，glStencilMask(0xff);
	http://www.cnblogs.com/aokman/archive/2010/12/13/1904723.html
*/
#endif // RENDERER_INCLUDE_RENDERSYSTEM_OUTLINE_H
