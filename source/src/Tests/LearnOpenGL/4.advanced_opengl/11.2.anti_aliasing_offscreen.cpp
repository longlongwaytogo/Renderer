//11.2.anti_aliasing_offscreen.cpp
#include<TestOpenGLBase.h>
#include <box.h>
#include <utils/FBO.h>
#include <scene/QuadGeometry.h>

using namespace RenderSystem;
class anti_aliasing_offscreen :public TestBase
{
	virtual void InitShader()
	{
		TestBase::InitShader();
		std::string shaderPath = FileSystem::getPath("resources/shaders/tests/LearnOpenGL/Shaders/4.advanced_opengl/");
		m_quadShader.loadFile(shaderPath + "11.2.anti_aliasing_offscreen.vs", shaderPath + "11.2.anti_aliasing_offscreen.ps");
	}
	virtual void InitData()
	{
		TestBase::InitData();
		m_fbo.CreateFbo();
		m_fbo.AddColorAttachment(GL_COLOR_ATTACHMENT0,SCR_WIDTH, SCR_HEIGHT, true, 4);
		m_fbo.AddDepthStencilAttachment(SCR_WIDTH, SCR_HEIGHT, true, 4);
		if (!m_fbo.CheckFboComplete())
		{
			terminate();
		}
		m_resoluationFbo.CreateFbo();
		m_resoluationFbo.AddColorAttachment(GL_COLOR_ATTACHMENT0);
		if (!m_resoluationFbo.CheckFboComplete())
		{
			terminate();
		}
		m_quad.Init();
	}
	virtual void Render()
	{
		glEnable(GL_DEPTH_TEST);
		
		m_fbo.Enable(true);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		TestBase::Render();
		m_fbo.Enable(false);
#define OUTPUT_WINDOW 0
#if OUTPUT_WINDOW
		// copy to defualt window buffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo.GetFboID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH,SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
#else  // output innerfbo and draw window by quad
		m_resoluationFbo.Enable(true);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo.GetFboID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resoluationFbo.GetFboID());
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		m_resoluationFbo.Enable(false);
		
		m_quadShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_resoluationFbo.GetTexture(GL_COLOR_ATTACHMENT0));
		m_quad.Draw();

#endif 
	}
private:
	Fbo m_fbo;
	Fbo m_resoluationFbo;
	QuadGeometry m_quad;
	Shader m_quadShader;
};
START_TEST(anti_aliasing_offscreen)
