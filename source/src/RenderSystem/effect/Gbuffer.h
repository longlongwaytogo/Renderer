//////////////////////////////////////////////////////////////////////////
//@file: GBuffer.h
//@brief: 封装GBuffer相关操作和接口
//@author: szh
//@date: 2018/12/26

#ifndef RENDERER_INCLUDE_RENDERSYSTEM_GBUFFER_H
#define RENDERER_INCLUDE_RENDERSYSTEM_GBUFFER_H

#include <RenderSystem/Export.h>

#include <iostream>
#include <comm/glad/glad.h>
#include <debugger.h>

namespace RenderSystem {

	class  /*RENDERSYSTEM_API*/ GBuffer
	{
	public:

		GBuffer(int w = 1, int h = 1) :m_gBufferId(-1),
			m_gPosition(-1),
			m_gNormal(-1),
			m_gAlbedoSpec(-1),
			m_width(w),
			m_height(h)
		{

		}

		~GBuffer()
		{
			glDeleteBuffers(1, &m_gBufferId);
			glDeleteRenderbuffers(1, &m_gDepth);
		}

		void Create(int width, int height)
		{
			m_width = width;
			m_height = height;

			glGenFramebuffers(1, &m_gBufferId);
			glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferId);

			// position
			glGenTextures(1, &m_gPosition);
			glBindTexture(GL_TEXTURE_2D, m_gPosition);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPosition, 0);

			// normal
			glGenTextures(1, &m_gNormal);
			glBindTexture(GL_TEXTURE_2D, m_gNormal);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);

			// Albedospec
			glGenTextures(1, &m_gAlbedoSpec);
			glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoSpec, 0);

			// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
			unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);

			// create and attach depth buffer 
			glGenRenderbuffers(1, &m_gDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, m_gDepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gDepth);

			// 创建包含蒙版缓冲的方法：  https://www.jianshu.com/p/e7c9cf754c53
			//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height); 
			//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_gDepth);

			// final check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "FrameBuffer not complete!" << std::endl;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		/*void SetSize(int w, int h);*/

		//void RenderSystem::GBuffer::SetSize(int w, int h)
		//{
		//	m_width = w;
		//	m_height = h;
		//}
		void Enable(bool bEnable)
		{
			if (bEnable)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferId);
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}

		unsigned int GetFrameBufferID() { return m_gBufferId; }
		unsigned int GetPositionTexID() { return m_gPosition; }
		unsigned int GetNormalTexID() { return m_gNormal; }
		unsigned int GetAlbedoSpecTexID() { return m_gAlbedoSpec; }
		unsigned int GetDepthTexID() { return m_gDepth; }
	private:
		unsigned int m_gBufferId;	// OpenGL FBO ID
		unsigned int m_gPosition;	// OpenGL Texture ID for Position
		unsigned int m_gNormal;		// OpenGL Texture ID for Normal
		unsigned int m_gAlbedoSpec; // OpenGL Texture ID for AlbedoSpec
		unsigned int m_gDepth;		// OpenGL FBO ID for Depth

		int m_width;
		int m_height;
	};
}
#endif // RENDERER_INCLUDE_RENDERSYSTEM_GBUFFER_H