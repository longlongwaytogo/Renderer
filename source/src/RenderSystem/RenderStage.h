//////////////////////////////////////////////////////////////////////////
//@file: RenderStage.h
//@brief: 场景渲染的主要主体
//@author: szh 
//@date； 2018/12/26
#ifndef RENDERER_INCLUDE_RENDERSYSTEM_RENDERSTAGE_H
#define RENDERER_INCLUDE_RENDERSYSTEM_RENDERSTAGE_H
#include <RenderSystem/Export.h>
#include <RenderSystem/RenderWindow.h>
#include <memory.h>
#include <comm/shader_m.h>
#include <comm/model.h>
#include <comm/camera.h>

namespace RenderSystem
{
	class RENDERSYSTEM_API RenderStage
	{
	public:
		RenderStage(RenderWindowPtr window);
		~RenderStage();

		bool Init();

		bool LoadData();

		void RenderLoop();

		void UpdateEvent(float deltaTime);

		void UpdateData();

		void DoRender();

		void Stop();

		Camera& GetCmaera();
	private:
		void TestRender();
		
	private:
		RenderWindowPtr m_window;
		float m_deltaTime;
		float m_lastFrame;
		bool  m_bRun;
		Model m_defualtModel;
		Shader m_defualtShader;
		Camera m_camera;

	};
	typedef std::shared_ptr<RenderStage> RenderStagePtr;

	
}

#endif // RENDERER_INCLUDE_RENDERSYSTEM_RENDERSTAGE_H