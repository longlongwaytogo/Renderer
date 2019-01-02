//////////////////////////////////////////////////////////////////////////
//@file FBO.h
//@brief: Ö¡»º³åÇø¶ÔÏó
//@author: szh
//@date: 2018/12/28
#ifndef RENDERER_INCLUDE_RENDERSYSTEM_FBO_H
#define RENDERER_INCLUDE_RENDERSYSTEM_FBO_H

#include <comm/glad/glad.h>
#include <map>


namespace RenderSystem
{
	class Fbo
	{
	public:
		Fbo() :m_fbo(GL_INVALID_VALUE)
		{
		}
		~Fbo()
		{
			glDeleteFramebuffers(1, &m_fbo);
		}

		void Enable(bool enable = true)
		{
			if (enable)
				glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			else
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		unsigned int GetTexture(GLenum attachmentId)
		{
			auto itr = m_colorAttachments.find(attachmentId);
			if (itr != m_colorAttachments.end())
				return itr->second;
			else
				return 0;
		}
		void AddDepthStencilAttachment(int w = 800, int h = 600, bool bMultiSample = false, int samples = 4)
		{
			// create a renderBuffer object for depth an stencil attachment
			glGenRenderbuffers(1, &m_depth_stencil);
			glBindRenderbuffer(GL_RENDERBUFFER, m_depth_stencil);
			if (bMultiSample)
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, w, h);
			else
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_stencil);
		}

		void AddRBOAttachmentt(GLenum attachmentId, int w = 800, int h = 600, bool bMultiSample = false, int samples = 4)
		{
			unsigned int rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			if(bMultiSample)
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA, w, h);
			else 
				glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, w, h);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentId, GL_RENDERBUFFER, rbo);
			m_colorAttachments[attachmentId] = rbo;
		}
		void AddColorAttachment(GLenum attachmentId, int w = 800, int h = 600,bool bMultiSample = false,int samples = 4)
		{
			// create a color attachment texture
			unsigned int texture;
			glGenTextures(1, &texture);
			if (bMultiSample)
			{
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, w, h, GL_TRUE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentId, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, texture);
				//glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentId, GL_TEXTURE_2D, texture, 0);
				//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
			}
			
			m_colorAttachments[attachmentId] = texture;
		}
		void CreateFbo()
		{
			glGenFramebuffers(1, &m_fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		}

		bool CheckFboComplete()
		{
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				std::cout << "gl fbo not complete!" << std::endl;
				return false;
			}
			return true;
		}

		void CreateDefualtFbo(int w = 800, int h = 600)
		{
			CreateFbo();
			AddColorAttachment( GL_COLOR_ATTACHMENT0,w,h);
			AddDepthStencilAttachment(w, h);
		}

		unsigned int GetFboID() { return m_fbo; }
	private:
		unsigned int m_fbo;
		std::map<unsigned int, unsigned int> m_colorAttachments;
		unsigned int m_depth_stencil;
	};
}

#endif // RENDERER_INCLUDE_RENDERSYSTEM_FBO_H