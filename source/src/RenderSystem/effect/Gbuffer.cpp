
#include <RenderSystem/effect/Gbuffer.h>
#include <glfw/glfw3.h>

RenderSystem::GBuffer::GBuffer(int w, int h):m_gBufferId(-1),
m_gPosition(-1),
m_gNormal(-1),
m_gAlbedoSpec(-1),
m_width(w),
m_height(h)
{

}

RenderSystem::GBuffer::~GBuffer()
{
	glDeleteBuffers(m_gBufferId);
	
}

RenderSystem::GBuffer::Create()
{
	glGenFramebuffers(1, &m_gBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferId);

	// position
	glGenTextures(1, &m_gPosition);
	glBindTexture(GL_TEXTURE_2D, 0, m_gPosition);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPosition, 0);
	
	// normal
	glGenTextures(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, 0, m_gNormal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);

	// Albedospec
	glGenTextures(1, &m_gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, 0, m_gAlbedoSpec);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoSpec, 0);

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// create and attach depth buffer 
	glGenFramebuffers(1, &m_gDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, m_gDepth);

	// final check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FrameBuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

